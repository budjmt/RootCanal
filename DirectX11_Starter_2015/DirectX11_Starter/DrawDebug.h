#pragma once

#include <vector>

#include "DirectXGameCore.h"
#include "SimpleShader.h"

#include "MarchMath.h"
#include "DXInfo.h"
#include "Drawable.h"
#include "Camera.h"
#include "ModelHelper.h"
#include "Mesh.h"

#define DEBUG true

struct Sphere { vec3 center; float rad; };

class DrawDebug
{
public:
	static DrawDebug& getInstance();
	void camera(Camera** c);

	//this is the actual draw call
	void draw();

	//these are called externally for drawing stuff
	void drawDebugVector(vec3 start, vec3 end, vec3 color = vec3(0.7f, 1, 0));
	void drawDebugSphere(vec3 pos, float rad);
private:
	DrawDebug();
	~DrawDebug();
	DrawDebug(const DrawDebug&) = delete;
	void operator=(const DrawDebug&) = delete;

	//these are to separate the individual processes
	void drawVectors();
	void drawSpheres();

	Camera** cam = nullptr;

	Mesh *sphere, *arrow;
	int sphereVerts;

	SimpleVertexShader *vecVert, *meshVert;
	SimplePixelShader *vecPixel, *meshPixel;
	
	std::vector<vec3> debugVectors;
	std::vector<Sphere> debugSpheres;

	std::vector<DirectX::XMFLOAT3> vecBuffer, arrowBuffer;
	std::vector<DebugVertex> sphereBuffer;
};