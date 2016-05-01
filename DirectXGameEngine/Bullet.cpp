#include "Bullet.h"

Bullet::Bullet(Mesh * mesh, Material * material, float s, vec3 dir, Ship *sh)
	:ColliderObject(mesh, material)
{
	rigidBody().floating(true);
	transform.setBaseDirections(vec3(0, 1, 0), vec3(0, 0, -1));
	speed = s;
	direction = dir;
	ship = sh;
}

void Bullet::setSpeed(float s){ speed = s; }

void Bullet::update(float dt) {
	transform.forward() = direction;
	rigidBody().netForce += rigidBody().mass() * 50 * speed * transform.forward();
}

void Bullet::handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions)
{
	if (other->collider() == ship->collider()) {
		active = false;
		ship->addHealth(-1.f);
		transform.scale(vec3(0.f, 0.f, 0.f));
	}
}
