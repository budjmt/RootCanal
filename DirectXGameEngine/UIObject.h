#pragma once

#include "GameObject.h"

//UIObjects are GameObjects that have screen-space transforms
class UIObject : public GameObject
{
public:
	UIObject();
	UIObject(Drawable* s);
	UIObject(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s);

	virtual void update(float dt, Mouse* mouse) = 0;
	virtual void draw(ID3D11DeviceContext* deviceContext);
};

