#include "DrawDebug.h"
#include <iostream>
DrawDebug::DrawDebug() {
#if DEBUG
	for (int i = 0; i < 4; i++)
		debugVectors.push_back(vec3(0, 0, 0));

	DXInfo& d = DXInfo::getInstance();

	D3D11_RASTERIZER_DESC rdesc = d.rasterDesc;
	rdesc.FillMode = D3D11_FILL_WIREFRAME;
	d.device->CreateRasterizerState(&d.rasterDesc, &wireframe);
	rdesc.FillMode = D3D11_FILL_SOLID;
	d.device->CreateRasterizerState(&d.rasterDesc, &fill);

	vecVert = new SimpleVertexShader(d.device, d.deviceContext);
	vecVert->LoadShaderFile(L"DebugVecVertex.cso");
	vecPixel = new SimplePixelShader(d.device, d.deviceContext);
	vecPixel->LoadShaderFile(L"DebugVecPixel.cso");

	meshVert = new SimpleVertexShader(d.device, d.deviceContext);
	meshVert->LoadShaderFile(L"DebugMeshVertex.cso");
	meshPixel = new SimplePixelShader(d.device, d.deviceContext);
	meshPixel->LoadShaderFile(L"DebugMeshPixel.cso");

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

	//vertex buffer
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(DebugVertex) * arrow->meshBuffer().meshArray.size();
	vbd.CPUAccessFlags = 0;

	initialVertexData.pSysMem = &arrow->meshBuffer().meshArray[0];
	HR(d.device->CreateBuffer(&vbd, &initialVertexData, &avb));

	//instance buffer
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * MAX_VECTORS;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0; ibd.StructureByteStride = 0;
	
	initialVertexData.pSysMem = &DirectX::XMFLOAT4X4();
	HR(d.device->CreateBuffer(&ibd, &initialVertexData, &ainstb));

	//index buffer
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * arrow->meshBuffer().meshElementArray.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; ibd.CPUAccessFlags = 0;

	initialIndexData.pSysMem = &arrow->meshBuffer().meshElementArray[0];
	HR(d.device->CreateBuffer(&ibd, &initialIndexData, &aib));

	//sphere setup
	sphere = loadOBJ("Assets/_debug/sphere.obj");
	assert(sphere != nullptr);

	//vertex buffer
	vbd.ByteWidth = sizeof(DebugVertex) * sphere->meshBuffer().meshArray.size();
	initialVertexData.pSysMem = &sphere->meshBuffer().meshArray[0];
	HR(d.device->CreateBuffer(&vbd, &initialVertexData, &svb));

	//index buffer
	ibd.ByteWidth = sizeof(int) * arrow->meshBuffer().meshElementArray.size();
	initialIndexData.pSysMem = &sphere->meshBuffer().meshElementArray[0];
	HR(d.device->CreateBuffer(&ibd, &initialIndexData, &sib));

	//instance buffer
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * MAX_SPHERES;
	ibd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	initialVertexData.pSysMem = &DirectX::XMFLOAT4X4();
	HR(d.device->CreateBuffer(&ibd, &initialVertexData, &sinstb));
	
	debugVectors = std::vector<vec3>();
#endif
}

DrawDebug::~DrawDebug() {
#if DEBUG
	delete sphere; delete arrow;
	sphere = arrow = nullptr;
	svb->Release(); sib->Release(); sinstb->Release();
	avb->Release(); aib->Release(); ainstb->Release();
	vvb->Release();
	svb = sib = sinstb = avb = aib = ainstb = vvb = nullptr;
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
	DXInfo& d = DXInfo::getInstance();
	d.deviceContext->RSSetState(d.rasterState);
	d.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
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
			DirectX::XMFLOAT3{ sc.x, sc.y, sc.z }			
		});
		vecBufferData.push_back(DebugVector{
			DirectX::XMFLOAT3{ e.x, e.y, e.z },
			DirectX::XMFLOAT3{ ec.x, ec.y, ec.z }
		});

		arrowBufferData.push_back(DirectX::XMFLOAT4X4(&mat4::lookAt(e, e + e - s, vec3(0,0,-1))[0][0]));
	}

	DXInfo& d = DXInfo::getInstance();
	if ((*cam) != nullptr) (*cam)->updateCamMat(vecVert);

	//upload and draw vector part
	UINT vstride = sizeof(DebugVector), istride;
	UINT voffset = 0, ioffset = 0;
	d.deviceContext->IASetVertexBuffers(0, 1, &vvb, &vstride, &voffset);

	//upload
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	ZeroMemory(&mappedRes, sizeof(D3D11_MAPPED_SUBRESOURCE));
	d.deviceContext->Map(vvb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	memcpy(&mappedRes, &vecBufferData[0], vecBufferData.size() * vstride);
	d.deviceContext->Unmap(vvb, 0);
	
	//draw
	vecVert->SetShader(true);
	d.deviceContext->RSSetState(wireframe);
	d.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	d.deviceContext->Draw(vecBufferData.size(), 0);

	//upload and draw arrow part
	vstride = sizeof(DebugVertex); istride = sizeof(DirectX::XMFLOAT4X4);
	ID3D11Buffer* buffs[] = { avb, ainstb };
	UINT strides[] = { vstride, istride }, offsets[] = { voffset, ioffset };
	d.deviceContext->IASetVertexBuffers(0, 2, buffs, strides, offsets);
	d.deviceContext->IASetIndexBuffer(aib, DXGI_FORMAT_R32_UINT, 0);

	//upload
	ZeroMemory(&mappedRes, sizeof(D3D11_MAPPED_SUBRESOURCE));
	d.deviceContext->Map(ainstb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	memcpy(&mappedRes, &arrowBufferData[0], arrowBufferData.size() * istride);
	d.deviceContext->Unmap(ainstb, 0);

	//draw
	meshVert->SetShader(true);
	d.deviceContext->RSSetState(fill);
	d.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d.deviceContext->DrawIndexedInstanced(arrow->meshBuffer().meshElementArray.size(), arrowBufferData.size(), 0, 0, 0);

	debugVectors = std::vector<vec3>();
	arrows = std::vector<vec3>();
}

void DrawDebug::drawSpheres() {
	DXInfo& d = DXInfo::getInstance();
	if ((*cam) != nullptr) (*cam)->updateCamMat(meshVert);

	for (int i = 0, numSpheres = debugSpheres.size(); i < numSpheres; i++) {
		Sphere s = debugSpheres[i];
		mat4 translate, scale;
		translate = mat4::translate(s.center);
		scale = mat4::scale(vec3(1, 1, 1) * (s.rad * 2));
		sphereBufferData.push_back(DirectX::XMFLOAT4X4(&(scale * translate)[0][0]));
	}
	UINT vstride = sizeof(DebugVertex), istride = sizeof(DirectX::XMFLOAT4X4);
	UINT voffset = 0, ioffset = 0;
	ID3D11Buffer* buffs[] = { svb, sinstb };
	UINT strides[] = { vstride, istride }, offsets[] = { voffset, ioffset };
	d.deviceContext->IASetVertexBuffers(0, 2, buffs, strides, offsets);

	//upload
	D3D11_MAPPED_SUBRESOURCE mappedRes;
	ZeroMemory(&mappedRes, sizeof(D3D11_MAPPED_SUBRESOURCE));
	d.deviceContext->Map(sinstb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes);
	memcpy(&mappedRes, &sphereBufferData[0], sphereBufferData.size() * istride);
	d.deviceContext->Unmap(sinstb, 0);

	//draw
	meshVert->SetShader(true);
	d.deviceContext->RSSetState(fill);
	d.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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