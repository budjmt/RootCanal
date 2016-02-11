#pragma once

#include "Transform.h"

//#include <map>

const int FLOATS_PER_VERT = 3;
const int FLOATS_PER_NORM = 3;
const int FLOATS_PER_UV = 2;

class Drawable
{
public:
	Drawable();
	Drawable(const Drawable& other);
	virtual ~Drawable();
	uint32_t& shader;
	int& colorLoc;
	virtual void draw(float x, float y, float xScale, float yScale);
	virtual void draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot);
	virtual void draw(Transform t);
	void setWorldMatrix(vec3 pos, vec3 scale, vec3 rotAxis, float rot);
	//uint32_t genTexture(const char* texFile);
protected:
	uint32_t vBuffer;
	uint32_t vArray;
	uint32_t dshaderProg;
	int offset, scale, worldMatrix, dcolorLoc;
	//static std::map<const char*, GLuint> loadedTextures;//all currently loaded textures
};