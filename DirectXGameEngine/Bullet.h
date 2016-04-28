#pragma once

#include "ColliderObject.h"

class Bullet :
	public ColliderObject
{
public:
	Bullet(Mesh * mesh, Material * material, float s, vec3 dir);
	void setSpeed(float s);
	void update(float dt);
private:
	float speed;
	vec3 direction;
};