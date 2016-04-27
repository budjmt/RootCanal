#pragma once

#include "GameObject.h"
#include "Ship.h"
#include "OpacityRadius.h"

class ToothFront :
	public ColliderObject
{
public:
	ToothFront(Mesh* mesh, Material* material, Camera** cam);
	~ToothFront();

	void update(float dt, Mouse* m) {};
	void draw(ID3D11DeviceContext* deviceContext);

	Ship* ship;
	OpacityRadius opacityRadius;
	Drawable* opacity;
};

