#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"

#include "Transform.h"

//#include <map>

const int FLOATS_PER_VERT = 3;
const int FLOATS_PER_NORM = 3;
const int FLOATS_PER_UV = 2;

class Drawable
{
public:
	Drawable(); virtual ~Drawable();
	void vertexShader(SimpleVertexShader* s); void pixelShader(SimplePixelShader* s);
	virtual void draw(float x, float y, float z, float xScale, float yScale, float zScale, ID3D11DeviceContext* deviceContext);
	virtual void draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext);
	virtual void draw(Transform* t, ID3D11DeviceContext* deviceContext);
	void setWorldMatrix(vec3 pos, vec3 scale, vec3 rotAxis, float rot);
	//uint32_t genTexture(const char* texFile);
protected:
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* _vertexShader;
	SimplePixelShader* _pixelShader;

	//static std::map<const char*, GLuint> loadedTextures;//all currently loaded textures
};