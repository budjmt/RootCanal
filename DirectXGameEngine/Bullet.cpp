#include "Bullet.h"

#include "Cannon.h"

Bullet::Bullet(Mesh * mesh, Material * material, float s, vec3 dir, Ship *sh)
	:ColliderObject(mesh, material)
{
	rigidBody().floating(true);
	audioManager = sh->getAudioManager();
	transform.setBaseDirections(vec3(0, 1, 0), vec3(0, 0, -1));
	speed = s;
	direction = dir;
	ship = sh;
}

void Bullet::setSpeed(float s){ speed = s; }
void Bullet::setDir(vec3 d) { direction = d; }

void Bullet::update(float dt) {
	transform.forward() = direction;
	rigidBody().netForce += rigidBody().mass() * 200 * speed * direction;
	vec3 toParent = cannon->transform.getComputed().position() - transform.getComputed().position();
	if (vec3::dot(toParent, toParent) > LIFE_DIST * LIFE_DIST) {
		active = false;
		transform.scale(vec3(0.f, 0.f, 0.f));
		body.solid(0);
	}

    // TODO: Figure out why collisions aren't registering...
    if( active && collider()->aabb().intersects( ship->collider()->aabb() ) ) {
        active = false;
		audioManager->playFile(_TEXT("../Assets/Hit.wav"), 1.0, false);
        ship->addHealth( -5.f );
        transform.scale( vec3( 0.f, 0.f, 0.f ) );
        body.solid( 0 );
    }
}

void Bullet::handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions)
{
	/*if (other->collider() == ship->collider()) {
		active = false;
		ship->addHealth(-5.f);
		transform.scale(vec3(0.f, 0.f, 0.f));
		body.solid(0);
	}*/

    numCollisions--;
}
