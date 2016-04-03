#pragma once

#include "Game.h"

#include "Drawable.h"
#include "Mesh.h"

class DrawMesh : public Drawable
{
public:
    DrawMesh( Mesh* m, char* texturePath, ID3D11Device* device );
    DrawMesh( std::vector<vec3> v, std::vector<vec3> n, std::vector<vec3> u, Face f, char* texFile, ID3D11Device* device );
    ~DrawMesh();
    void DrawMesh::initBuffers( ID3D11Device* device );
    Mesh* mesh() const; void mesh( Mesh* m );
    //void setup(char* texFile, uint32_t shader);
    void draw( float x, float y, float z, float xScale, float yScale, float zScale, ID3D11DeviceContext* deviceContext );
    void draw( vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext );

private:
    Mesh* _mesh;
};