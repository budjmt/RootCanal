#pragma once

#include "DirectXGameCore.h"

#include "Drawable.h"
#include "Mesh.h"

class DrawMesh : Drawable
{
public:
	DrawMesh(Mesh* m, char* texFile, uint32_t shader);
	DrawMesh(std::vector<vec3> v, std::vector<vec3> n, std::vector<vec3> u, Face f, char* texFile, uint32_t shader);
	~DrawMesh();
	Mesh* mesh() const; void mesh(Mesh* m);
	//void setup(char* texFile, uint32_t shader);
	void draw(float x, float y, float xScale, float yScale, ID3D11DeviceContext* deviceContext);
	void draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext);

private:
	Mesh* _mesh;
};
