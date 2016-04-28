#pragma once

#include "ColliderObject.h"
#include "Ship.h"

class Bullet :
	public ColliderObject
{
public:
	Bullet(Mesh * mesh, Material * material, float speed, vec3 dir);
	void setSpeed(float s);
	void update(float dt);
private:
	float speed;
	vec3 direction;
};