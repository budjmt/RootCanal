#include "DrawMesh.h"

DrawMesh::DrawMesh(Mesh* m, char* texFile, ID3D11Device* device) { _mesh = m; initBuffers(device); }
DrawMesh::DrawMesh(std::vector<vec3> v, std::vector<vec3> n, std::vector<vec3> u, Face f, char* texFile, ID3D11Device* device) { _mesh = new Mesh(v, n, u, f); initBuffers(device); }
DrawMesh::~DrawMesh() { }
Mesh* DrawMesh::mesh() const { return _mesh; } void DrawMesh::mesh(Mesh* m) { _mesh = m; }

void DrawMesh::draw(float x, float y, float z, float xScale, float yScale, float zScale, ID3D11DeviceContext* deviceContext) {
	Drawable::draw(x, y, z, xScale, yScale, zScale, deviceContext);
	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	_material->setActive(true);
	deviceContext->DrawIndexed(
		_mesh->meshBuffer().meshElementArray.size(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
}

void DrawMesh::draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext) {
	Drawable::draw(pos, scale, rotAxis, rot, deviceContext);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	_material->setActive(true);
	deviceContext->DrawIndexed(_mesh->meshBuffer().meshElementArray.size(), 0, 0);
}

void DrawMesh::initBuffers(ID3D11Device* device) {
	MeshBuffer _meshBuffer = _mesh->meshBuffer();

	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * _meshBuffer.meshArray.size();       // 3 = number of vertices in the buffer
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Tells DirectX this is a vertex buffer
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = &_meshBuffer.meshArray[0];

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));



	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * _meshBuffer.meshElementArray.size();         // 3 = number of indices in the buffer
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER; // Tells DirectX this is an index buffer
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = &_meshBuffer.meshElementArray[0];

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer));
}