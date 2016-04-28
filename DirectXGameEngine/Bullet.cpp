#include "Bullet.h"

Bullet::Bullet(Mesh * mesh, Material * material, float s, vec3 dir)
	:ColliderObject(mesh, material)
{
	rigidBody().floating(true);
	transform.setBaseDirections(vec3(0, 1, 0), vec3(0, 0, -1));
	speed = s;
	direction = dir;
}

void Bullet::setSpeed(float s){ speed = s; }

void Bullet::update(float dt) {
	transform.forward() = direction;
	rigidBody().netForce += rigidBody().mass() * 50 * speed * transform.forward();
}
