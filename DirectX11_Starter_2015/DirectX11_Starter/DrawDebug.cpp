#include "DrawDebug.h"
#include <iostream>
DrawDebug::DrawDebug() {
#if DEBUG
	for (int i = 0; i < 4; i++)
		debugVectors.push_back(vec3(0, 0, 0));

	DXInfo& d = DXInfo::getInstance();

	//vecShader = loadShaderProgram("Shaders/_debug/vecvertexShader.glsl", "Shaders/_debug/vecfragmentShader.glsl");
	//meshShader = loadShaderProgram("Shaders/_debug/meshvertexShader.glsl", "Shaders/_debug/meshfragmentShader.glsl");

	//vector setup
	D3D11_BUFFER_DESC vbd, ibd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	//vbd.ByteWidth = sizeof(Vertex) * _meshBuffer.meshArray.size();//this is going to be a bit weird
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	vbd.MiscFlags = 0; vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData, initialIndexData;
	initialVertexData.pSysMem = &debugVectors[0];
	HR(d.device->CreateBuffer(&vbd, &initialVertexData, &asdafsdjlfdslk));

	//arrow setup
	arrow = loadOBJ("Assets/_debug/arrow.obj");
	assert(arrow != nullptr);

	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * arrow->meshBuffer().meshArray.size();
	vbd.CPUAccessFlags = 0;

	initialVertexData.pSysMem = &arrow->meshBuffer().meshArray[0];
	HR(d.device->CreateBuffer(&vbd, &initialVertexData, &avb));

	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * arrow->meshBuffer().meshElementArray.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; ibd.CPUAccessFlags = 0; ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	initialIndexData.pSysMem = &arrow->meshBuffer().meshElementArray[0];
	HR(d.device->CreateBuffer(&ibd, &initialIndexData, &aib));

	//sphere setup
	sphere = loadOBJ("Assets/_debug/sphere.obj");
	assert(sphere != nullptr);

	vbd.ByteWidth = sizeof(Vertex) * sphere->meshBuffer().meshArray.size();
	initialVertexData.pSysMem = &sphere->meshBuffer().meshArray[0];
	HR(d.device->CreateBuffer(&vbd, &initialVertexData, &svb));

	ibd.ByteWidth = sizeof(int) * arrow->meshBuffer().meshElementArray.size();
	initialIndexData.pSysMem = &sphere->meshBuffer().meshElementArray[0];
	HR(d.device->CreateBuffer(&ibd, &initialIndexData, &sib));
	
	debugVectors = std::vector<vec3>();
#endif
}

DrawDebug::~DrawDebug() {
#if DEBUG
	glDeleteBuffers(1, &vecBuffer);
	glDeleteBuffers(1, &arrowBuffer);
	glDeleteBuffers(1, &sphereBuffer);
#endif
}

DrawDebug& DrawDebug::getInstance() {
	static DrawDebug instance;
	return instance;
}

void DrawDebug::camera(Camera** c) { cam = c; }

void DrawDebug::draw() {
#if DEBUG
	drawVectors();
	drawSpheres();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
}

void DrawDebug::drawVectors() {
	for (int i = 0; i < 4; i++)	debugVectors.push_back(vec3());
	
	std::vector<vec3> arrows;
	for (int i = 0; i < 6; i++) arrows.push_back(vec3());
	
	int numVecs = debugVectors.size();
	for (int i = 0; i < numVecs; i += 4) {
		vec3 s = debugVectors[i],		c1 = debugVectors[i + 1]
		   , e = debugVectors[i + 2],	c2 = debugVectors[i + 3];
		vec3 v = e - s;
		v *= 0.05f;
		v = e - v;
		arrows.push_back(v + vec3(-1,0,-1) * 0.008f);
		arrows.push_back(c1);

		arrows.push_back(v + vec3(1,0,1)   * 0.008f);
		arrows.push_back(c1);

		arrows.push_back(e);
		arrows.push_back(c2);
	}

	DXInfo& d = DXInfo::getInstance();
	if ((*cam) != nullptr) (*cam)->updateCamMat(vecVert);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//upload and draw vector part
	for (auto v : debugVectors) vecBuffer.push_back(DirectX::XMFLOAT3{v.x, v.y, v.z});
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d.deviceContext->IASetVertexBuffers(0, 1, &vecBuffer, &stride, &offset);
	d.deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	vecVert->SetShader(true);
	d.deviceContext->DrawIndexed(vecBuffer.size(), 0, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//upload and draw arrow part
	for (auto a : arrows) arrowBuffer.push_back(DirectX::XMFLOAT3{ a.x,a.y,a.z });
	meshVert->SetShader(true);
	d.deviceContext->DrawIndexed(arrowBuffer.size(), 0, 0);

	debugVectors = std::vector<vec3>();
	arrows = std::vector<vec3>();
}

void DrawDebug::drawSpheres() {
	DXInfo& d = DXInfo::getInstance();
	if ((*cam) != nullptr) (*cam)->updateCamMat(meshVert);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glBindVertexArray(meshVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, sphereBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereElBuffer);

	int numSpheres = debugSpheres.size();
	for (int i = 0; i < numSpheres; i++) {
		Sphere s = debugSpheres[i];
		mat4 translate, scale;
		translate = mat4::translate(s.center);
		scale = mat4::scale(vec3(1, 1, 1) * (s.rad * 2));
		meshVert->SetMatrix4x4("world", &(scale * translate)[0][0]);
		meshVert->SetShader(true);
		d.deviceContext->DrawIndexed(sphereBuffer.size(), 0, 0);
	}

	debugSpheres = std::vector<Sphere>();
}

void DrawDebug::drawDebugVector(vec3 start, vec3 end, vec3 color) {
#if DEBUG
	DrawDebug& d = DrawDebug::getInstance();
	d.debugVectors.push_back(start);
	d.debugVectors.push_back(color);
	d.debugVectors.push_back(end);
	d.debugVectors.push_back(color);
#endif
}

void DrawDebug::drawDebugSphere(vec3 pos, float rad) {
#if DEBUG
	Sphere s = { pos, rad };
	DrawDebug& d = DrawDebug::getInstance();
	d.debugSpheres.push_back(s);
	//drawDebugVector(pos, pos + vec3(rad, 0, 0));
#endif
}