#pragma once
#include "Drawable.h"
class DrawScreen :
	public Drawable
{
public:
	DrawScreen();
	~DrawScreen();

	void draw(float x, float y, float z, float xScale, float yScale, float zScale, ID3D11DeviceContext* deviceContext);
	void draw(vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext);
};

