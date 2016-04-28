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

void Ship::addHealth(float hp){ health += hp; }

void Ship::update(float dt)
{
	processMovement(dt);

	/*
	if (health < 0) {
		sceneManager.SetScene(new MenuScene(&cam));
	}
	*/
}

void Ship::processMovement(float dt)
{
	Keyboard& keys = Keyboard::getInstance();

	if (keys.isDown(VK_LEFT)) {
		rigidBody().netAngAccel += vec3(0, 0, -PI * 2 * 15);
	}
	else if (keys.isDown(VK_RIGHT)) {
		rigidBody().netAngAccel += vec3(0, 0, PI * 2 * 15);
	}

	if (keys.isDown(VK_UP)) {
		rigidBody().netForce += rigidBody().mass() * 150 * transform.forward();
	}
}
