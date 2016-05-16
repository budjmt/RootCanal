#pragma once

#include "ColliderObject.h"
#include "Ship.h"

#define LIFE_DIST 30

class Cannon;

class Bullet :
	public ColliderObject
{
public:
	Bullet(Mesh * mesh, Material * material, float s, vec3 dir, Ship *sh);
	void setSpeed(float s);
	void setDir(vec3 dir);
	void update(float dt);
	void handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions);

	Cannon* cannon;
private:
	float speed;
	vec3 direction = vec3();
	Ship *ship;
	AudioManager* audioManager;
};