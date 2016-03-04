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
	vbd.ByteWidth = sizeof(DebugVector) * MAX_VECTORS;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; 
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 
	vbd.MiscFlags = 0; vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initialVertexData, initialIndexData;
	initialVertexData.pSysMem = &debugVectors[0];
	HR(d.device->CreateBuffer(&vbd, &initialVertexData, &vvb));

	//arrow setup
	arrow = loadOBJ("Assets/_debug/arrow.obj");
	assert(arrow != nullptr);

	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(DebugVertex) * arrow->meshBuffer().meshArray.size();
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

	vbd.ByteWidth = sizeof(DebugVertex) * sphere->meshBuffer().meshArray.size();
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
	//glDeleteBuffers(1, &vecBuffer);
	//glDeleteBuffers(1, &arrowBuffer);
	//glDeleteBuffers(1, &sphereBuffer);
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
	for (int i = 0, numVecs = debugVectors.size(); i < numVecs; i += 4) {
		vec3 s = debugVectors[i], sc = debugVectors[i + 1], e = debugVectors[i + 2], ec = debugVectors[i + 3];
		vecBufferData.push_back(DebugVector{
			DirectX::XMFLOAT3{ s.x, s.y, s.z },
			DirectX::XMFLOAT3{ sc.x, sc.y, sc.z },
			DirectX::XMFLOAT3{ e.x, e.y, e.z },
			DirectX::XMFLOAT3{ ec.x, ec.y, ec.z }
		});

		arrowBufferData.push_back(DirectX::XMFLOAT4X4(&mat4::lookAt(vec3(), e - s, vec3(0,0,-1))[0][0]));
	}

	DXInfo& d = DXInfo::getInstance();
	if ((*cam) != nullptr) (*cam)->updateCamMat(vecVert);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//draw vector part
	UINT stride = sizeof(DebugVector);
	UINT offset = 0;
	d.deviceContext->IASetVertexBuffers(0, 1, &vvb, &stride, &offset);
	vecVert->SetShader(true);
	d.deviceContext->Draw(vecBufferData.size(), 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw arrow part
	stride = sizeof(DebugVertex);
	d.deviceContext->IASetVertexBuffers(0, 2, &avb, &stride, &offset);
	stride = sizeof(DirectX::XMFLOAT4X4);
	d.deviceContext->IASetIndexBuffer(aib, DXGI_FORMAT_R32_UINT, 0);
	meshVert->SetShader(true);
	d.deviceContext->DrawIndexedInstanced(arrow->meshBuffer().meshElementArray.size(), arrowBufferData.size(), 0, 0, 0);

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
		sphereBufferData.push_back(DirectX::XMFLOAT4X4(&(scale * translate)[0][0]));
	}
	meshVert->SetShader(true);
	d.deviceContext->DrawIndexedInstanced(sphere->meshBuffer().meshElementArray.size(), sphereBufferData.size(), 0, 0, 0);

	debugSpheres = std::vector<Sphere>();
}

void DrawDebug::drawDebugVector(vec3 start, vec3 end, vec3 color) {
#if DEBUG
	debugVectors.push_back(start);
	debugVectors.push_back(color);
	debugVectors.push_back(end);
	debugVectors.push_back(color);
#endif
}

void DrawDebug::drawDebugSphere(vec3 pos, float rad) {
#if DEBUG
	Sphere s = { pos, rad };
	DrawDebug& d = DrawDebug::getInstance();
	debugSpheres.push_back(s);
	//drawDebugVector(pos, pos + vec3(rad, 0, 0));
#endif
}