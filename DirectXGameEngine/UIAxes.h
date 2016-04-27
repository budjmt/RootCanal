#pragma once

#include "UIObject.h"
#include "Camera.h"

class UIAxes : public UIObject
{
public:
	UIAxes();
	~UIAxes();

	void update(float dt, Mouse* mouse);
	void draw(ID3D11DeviceContext* deviceContext);

	Camera** camera;
};

