#include "Bullet.h"

Bullet::Bullet(Mesh * mesh, Material * material, float speed, vec3 dir)
	:ColliderObject(mesh, material)
{
	setSpeed(speed);
	direction = dir;
}

void Bullet::setSpeed(float s){ speed = s; }

void Bullet::update(float dt) {
	rigidBody().netAngAccel += (direction * speed * dt);
	rigidBody().netForce += rigidBody().mass() * 15 * transform.forward();
}
