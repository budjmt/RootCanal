#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<vec3> v, std::vector<vec3> n, std::vector<vec3> u, Face f)
{
	verts(v);
	normals(n);
	uvs(u);
	faces(f);

	h_dims = vec3(-1, -1, -1);
}

Mesh::~Mesh() { }

const std::vector<vec3>& Mesh::verts() const { return _verts; } void Mesh::verts(std::vector<vec3>& v) { _verts = v; }
const std::vector<vec3>& Mesh::uvs() const { return _uvs; } void Mesh::uvs(std::vector<vec3>& u) { _uvs = u; }
const std::vector<vec3>& Mesh::normals() const { return _normals; } void Mesh::normals(std::vector<vec3>& n) { _normals = n; }
Face Mesh::faces() const { return _faces; } void Mesh::faces(Face& f) { _faces = f; }

const MeshBuffer& Mesh::meshBuffer() const { return _meshBuffer; }

float getDistSq(vec3 v1, vec3 v2) {
	float xDist = v1.x - v2.x;
	float yDist = v1.y - v2.y;
	float zDist = v1.z - v2.z;
	return xDist * xDist + yDist * yDist + zDist * zDist;
}

vec3 Mesh::getDims() {
	//right now this assumes the model is centered at 0,0,0
	if (h_dims.x > 0)
		return h_dims;
	vec3 center = vec3(0, 0, 0);
	//find the most distant point from the center
	vec3 max = _verts[0];
	float maxDistSq = getDistSq(max, center);
	for (int i = 1, numVerts = _verts.size(); i < numVerts; i++) {
		vec3 v = _verts[i];
		float distSq = getDistSq(v, center);
		if (distSq > maxDistSq) { max = v; maxDistSq = distSq; }
	}
	//this finds the most distant point from THAT most distant point
	vec3 min = _verts[0];
	maxDistSq = getDistSq(min, max);
	for (int i = 1, numVerts = _verts.size(); i < numVerts; i++) {
		vec3 v = _verts[i];
		float distSq = getDistSq(v, max);
		if (distSq > maxDistSq) {
			min = v;
			maxDistSq = distSq;
		}
	}

	float radius = vec3::length(max - min) * 0.5f;
	vec3 d = vec3(radius, radius, radius);
	h_dims = d;
	return h_dims;
}

MeshBuffer Mesh::genMeshArrays() {
	MeshBuffer m;
	for (uint32_t i = 0, numfaceVerts = _faces.verts.size(); i < numfaceVerts; i++) {
		bool inArr = false;
		uint32_t index;
		for (index = 0; !inArr && index < _faces.combinations.size(); index++) {
			if ((uint32_t)_faces.combinations[index].x == _faces.verts[i]
				&& (uint32_t)_faces.combinations[index].y == _faces.uvs[i]
				&& (uint32_t)_faces.combinations[index].z == _faces.normals[i]) {
				inArr = true;
				index--;
			}
		}
		if (!inArr) {
			_faces.combinations.push_back(vec3((float)_faces.verts[i], (float)_faces.uvs[i], (float)_faces.normals[i]));
			m.meshArray.push_back(Vertex{
				DirectX::XMFLOAT3(_verts[_faces.verts[i]].x, _verts[_faces.verts[i]].y, _verts[_faces.verts[i]].z),
				DirectX::XMFLOAT2(_uvs[_faces.uvs[i]].x, _uvs[_faces.uvs[i]].y),
				DirectX::XMFLOAT3(_normals[_faces.normals[i]].x, _normals[_faces.normals[i]].y, _normals[_faces.normals[i]].z),
				DirectX::XMFLOAT4(rand() % 1000 / 1000.f, rand() % 1000 / 1000.f, rand() % 1000 / 1000.f, 1.f)
			});
		}
		m.meshElementArray.push_back(index);
	}
	return m;
}

void Mesh::initBuffers(ID3D11Device* device, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer) {
	_meshBuffer = genMeshArrays();

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