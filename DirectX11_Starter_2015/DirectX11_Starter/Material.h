#pragma once

#include "DirectXGameCore.h"
#include "SimpleShader.h"

#include "MarchMath.h"
#include "Camera.h"

class Material
{
public:
	Material();
	~Material();
	void vertexShader(SimpleVertexShader* v); 
	void pixelShader(SimplePixelShader* p);
	void camera(Camera** c);

	void updateMaterial(mat4* world);
	void setActive(bool b);

private:
	SimpleVertexShader* vertex;
	SimplePixelShader* pixel;
	Camera** _camera;
};

