#include "Ship.h"

Ship::Ship(Mesh * mesh, Material * material)
	: ColliderObject(mesh, material)
{
	rigidBody().floating(true);
	transform.setBaseDirections(vec3(0, 1, 0), vec3(0, 0, -1));
	health = 100;
}

float Ship::getHealth()
{
    return health;
}

void Ship::addHealth(float hp){ health += hp; }

void Ship::update(float dt)
{
	processMovement(dt);
}

void Ship::processMovement(float dt)
{
	Keyboard& keys = Keyboard::getInstance();

	if (keys.isDown(VK_LEFT)) {
		rigidBody().netAngAccel += vec3(0, 0, -PI * 2 * 15 / dt / 100);
	}
	else if (keys.isDown(VK_RIGHT)) {
		rigidBody().netAngAccel += vec3(0, 0, PI * 2 * 15 / dt / 100);
	}

	if (keys.isDown(VK_UP)) {
		rigidBody().netForce += rigidBody().mass() * 150 * transform.forward() / dt / 100;
	}

	vec3 shipPos = transform.position();
	if (shipPos.x > MAX_X || shipPos.x < -MAX_X)
		shipPos.x *= -1;
	if((shipPos.y > MAX_Y || shipPos.y < -MAX_Y))
		shipPos.y *= -1;
	transform.position(shipPos);
}

void Ship::handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions)
{
    numCollisions--;
}