#pragma once

#include "ColliderObject.h"
#include "RootCanal.h"

class Ship :
	public ColliderObject
{
public:
	Ship(Mesh* mesh, Material* material);
	Ship(vec3 p, vec3 dims, vec3 sc, vec3 rA, float r, Drawable* s);

	void addHealth(float hp);
	void update();
	void processMovement(float dt);
private:
	float health;
	Camera* cam;
};