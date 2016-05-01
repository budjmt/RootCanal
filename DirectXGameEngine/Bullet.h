#pragma once

#include "ColliderObject.h"
#include "Ship.h"

class Bullet :
	public ColliderObject
{
public:
	Bullet(Mesh * mesh, Material * material, float s, vec3 dir, Ship *sh);
	void setSpeed(float s);
	void update(float dt);
	void handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions);
private:
	float speed;
	vec3 direction;
	Ship *ship;
};