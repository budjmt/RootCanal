#pragma once

#include "ColliderObject.h"

#define MAX_X 75
#define MAX_Y 75

class Ship :
	public ColliderObject
{
public:
	Ship(Mesh* mesh, Material* material);
	//Ship(vec3 p, vec3 dims, vec3 sc, vec3 rA, float r, Drawable* s);

	void addHealth(float hp);
	void update(float dt);
	void processMovement(float dt);
	void handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions);
private:
	float health;
	Camera* cam;
};