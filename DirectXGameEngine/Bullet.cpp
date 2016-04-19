#include "Bullet.h"

Bullet::Bullet(Mesh * mesh, Material * material, float speed)
	:ColliderObject(mesh, material)
{
	setSpeed(speed);
}

void Bullet::setSpeed(float s){ speed = s; }

void Bullet::update(float dt) {
	rigidBody().netAngAccel += (direction * speed * dt);
	rigidBody().netForce += rigidBody().mass() * 50000 * dt * transform.forward();
}
