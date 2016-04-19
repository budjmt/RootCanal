#include "Ship.h"

Ship::Ship(Mesh * mesh, Material * material)
	: ColliderObject(mesh, material)
{
	rigidBody().floating(true);
	transform.setBaseDirections(vec3(0, 1, 0), vec3(0, 0, -1));
	health = 100;
}

Ship::Ship(vec3 p, vec3 dims, vec3 sc, vec3 rA, float r, Drawable* s)
	: ColliderObject(p, dims, sc, rA, r, s)
{
	rigidBody().floating(true);
	transform.setBaseDirections(vec3(0, 1, 0), vec3(0, 0, -1));
	health = 100;
}

void Ship::processMovement(float dt)
{
	Keyboard& keys = Keyboard::getInstance();

	if (keys.isDown(VK_LEFT)) {
		rigidBody().netAngAccel += vec3(0, 0, -PI * 2 * 10000 * dt);
	}
	else if (keys.isDown(VK_RIGHT)) {
		rigidBody().netAngAccel += vec3(0, 0, PI * 2 * 10000 * dt);
	}

	if (keys.isDown(VK_UP)) {
		rigidBody().netForce += rigidBody().mass() * 50000 * dt * transform.forward();
	}
}
