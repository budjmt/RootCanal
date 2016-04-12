#include "Mesh.h"

#include <iostream>

#include "MeshImporter.h"

Mesh::Mesh( std::vector<vec3> v, std::vector<vec3> n, std::vector<vec3> u, Face f )
{
    _verts = v;
    _normals = n;
    _uvs = u ;
    _faces = f;

    h_dims = vec3( -1, -1, -1 );
    _meshBuffer = genMeshArrays();
}

std::map<std::string, Mesh*> Mesh::loadedMeshes;//all currently loaded meshes

const std::vector<vec3>& Mesh::verts() const { return _verts; } void Mesh::verts( std::vector<vec3>& v ) { _verts = v; }
const std::vector<vec3>& Mesh::uvs() const { return _uvs; } void Mesh::uvs( std::vector<vec3>& u ) { _uvs = u; }
const std::vector<vec3>& Mesh::normals() const { return _normals; } void Mesh::normals( std::vector<vec3>& n ) { _normals = n; }
Face Mesh::faces() const { return _faces; } void Mesh::faces( Face& f ) { _faces = f; }

const MeshBuffer& Mesh::meshBuffer() const { return _meshBuffer; }

float getDistSq( vec3 v1, vec3 v2 ) {
    float xDist = v1.x - v2.x;
    float yDist = v1.y - v2.y;
    float zDist = v1.z - v2.z;
    return xDist * xDist + yDist * yDist + zDist * zDist;
}

vec3 Mesh::getDims() {
    //right now this assumes the model is centered at 0,0,0
    if( h_dims.x > 0 )
        return h_dims;
    vec3 center = vec3( 0, 0, 0 );
    //find the most distant point from the center
    vec3 max = _verts[0];
    float maxDistSq = getDistSq( max, center );
    for( int i = 1, numVerts = _verts.size(); i < numVerts; i++ ) {
        vec3 v = _verts[i];
        float distSq = getDistSq( v, center );
        if( distSq > maxDistSq ) { max = v; maxDistSq = distSq; }
    }
    //this finds the most distant point from THAT most distant point
    vec3 min = _verts[0];
    maxDistSq = getDistSq( min, max );
    for( int i = 1, numVerts = _verts.size(); i < numVerts; i++ ) {
        vec3 v = _verts[i];
        float distSq = getDistSq( v, max );
        if( distSq > maxDistSq ) {
            min = v;
            maxDistSq = distSq;
        }
    }

    float radius = vec3::length( max - min ) * 0.5f;
    vec3 d = vec3( radius, radius, radius );
    h_dims = d;
    return h_dims;
}

vec3 Mesh::getCentroid() {
    vec3 average;
    for( auto v : _verts ) average += v;
    average /= (float)_verts.size();

    //rounding niceness, change sig to the place you want to round to, e.g. 100 is round to hundredths
    float sig = 100.f;
    for( int i = 0; i < 3; i++ ) average[i] = roundf( average[i] * sig ) / sig;

    return average;
}

void Mesh::adjustLoc( vec3 d ) {
    for( int i = 0, numVerts = _verts.size(); i < numVerts; i++ ) _verts[i] += d;
}

MeshBuffer Mesh::genMeshArrays() {
    MeshBuffer m;
    for( uint32_t i = 0, numfaceVerts = _faces.verts.size(); i < numfaceVerts; i++ ) {
        bool inArr = false;
        uint32_t index;
        //TODO: fix this bottleneck! It's super duper slow!
        for( index = 0; !inArr && index < _faces.combinations.size(); index++ ) {
            if( (uint32_t)_faces.combinations[index].x == _faces.verts[i]
                && (uint32_t)_faces.combinations[index].y == _faces.uvs[i]
                && (uint32_t)_faces.combinations[index].z == _faces.normals[i] ) {
                inArr = true;
                index--;
            }
        }
        if( !inArr ) {
            _faces.combinations.push_back( vec3( (float)_faces.verts[i], (float)_faces.uvs[i], (float)_faces.normals[i] ) );
			Vertex v = {
				DirectX::XMFLOAT4(rand() % 1000 / 1000.f, rand() % 1000 / 1000.f, rand() % 1000 / 1000.f, 1.f),
				DirectX::XMFLOAT3(_verts[_faces.verts[i]].x, _verts[_faces.verts[i]].y, _verts[_faces.verts[i]].z),
				DirectX::XMFLOAT3(_normals[_faces.normals[i]].x, _normals[_faces.normals[i]].y, _normals[_faces.normals[i]].z),
				DirectX::XMFLOAT2(_uvs[_faces.uvs[i]].x, _uvs[_faces.uvs[i]].y)
			};
            m.meshArray.push_back( v );
        }
        m.meshElementArray.push_back( index );
    }
    return m;
}

Mesh* Mesh::getMesh( const char * key )
{
    return loadedMeshes.at( key );
}

Mesh* Mesh::createMesh( const char* meshFile )
{
    MeshImporter meshImporter;
    Mesh* loadedMesh = meshImporter.loadMesh( meshFile );

    assert( loadedMesh != nullptr );

    loadedMeshes[meshFile] = loadedMesh;
    return loadedMesh;
}
