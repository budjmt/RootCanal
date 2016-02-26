#pragma once

#include "DirectXGameCore.h"

#include <vector>

#include "Vertex.h"

#include "MarchMath.h"
//#include "Drawable.h"

struct MeshBuffer {
	std::vector<Vertex> meshArray = std::vector<Vertex>();
	std::vector<uint32_t> meshElementArray = std::vector<uint32_t>();
};

struct Face {
	//these all correspond to the indices in the vectors
	std::vector<uint32_t> verts, uvs, normals;
	std::vector<vec3> combinations;//all the unique v/u/n index combinations
};

class Mesh
{
public:
	Mesh(std::vector<vec3> v, std::vector<vec3> n, std::vector<vec3> u, Face f);
	~Mesh();
	const std::vector<vec3>& verts() const; void verts(std::vector<vec3>& v);
	const std::vector<vec3>& uvs() const; void uvs(std::vector<vec3>& u);
	const std::vector<vec3>& normals() const; void normals(std::vector<vec3>& n);
	Face faces() const; void faces(Face& f);

	const MeshBuffer& meshBuffer() const;
	vec3 getDims();

	MeshBuffer genMeshArrays();

protected:
	std::vector<vec3> _verts, _normals, _uvs;
	Face _faces;
	MeshBuffer _meshBuffer;

	vec3 h_dims;
	//friend class DrawMesh;
};