#pragma once

#include "Game.h"

#include <vector>
#include <map>

#include "Vertex.h"

#include "MarchMath.h"
//#include "Drawable.h"

struct MeshBuffer {
    std::vector<Vertex> meshArray;
    std::vector<uint32_t> meshElementArray;
};

struct Face {
    //these all correspond to the indices in the vectors
    std::vector<uint32_t> verts, uvs, normals;
    std::vector<vec3> combinations;//all the unique v/u/n index combinations
};

class Mesh
{
public:
    Mesh( std::vector<vec3> v, std::vector<vec3> n, std::vector<vec3> u, Face f );
    const std::vector<vec3>& verts() const; void verts( std::vector<vec3>& v );
    const std::vector<vec3>& uvs() const; void uvs( std::vector<vec3>& u );
    const std::vector<vec3>& normals() const; void normals( std::vector<vec3>& n );
    Face faces() const; void faces( Face& f );

    const MeshBuffer& meshBuffer() const;
    vec3 getDims();
    vec3 getCentroid();
    void adjustLoc( vec3 d );

    MeshBuffer genMeshArrays();

    static std::map<std::string, Mesh*> loadedMeshes;//all currently loaded meshes
    static Mesh* getMesh( const char* key );
    static Mesh* createMesh( const char* meshFile );

protected:
    std::vector<vec3> _verts, _normals, _uvs;
    Face _faces;
    MeshBuffer _meshBuffer;

    vec3 h_dims;
    //friend class DrawMesh;
};