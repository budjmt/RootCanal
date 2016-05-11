#include "Cannon.h"

Cannon::Cannon(Mesh * mesh, Material * material, Mesh* bMesh, Material* bMaterial, State* gamestate, Ship* player)
	:ColliderObject(mesh, material)
{
	//body.floating(true);
	body.fixed(1.f);
	transform.setBaseDirections(vec3(0, 1, 0), vec3(0, 0, -1));
	reloadTime = 3.0f;
	reloadTimer = reloadTime;
	bulletMesh = bMesh;
	bulletMaterial = bMaterial;
	shouldShoot = false;
	state = gamestate;
	ship = player;
}

void Cannon::spawnBullets(int num) {
	for (; num; --num) {
		Bullet* bullet = new Bullet(bulletMesh, bulletMaterial, 0.5f, vec3(), ship);
		bullet->transform.position(vec3(num * (rand() % 10), num * (rand() % 20), num * (rand() % 5)));
		bullet->transform.scale(vec3(0.f, 0.f, 0.f));
		bullet->rigidBody().solid(0);
		bullet->active = false;
		bullet->cannon = this;
		bullets.push_back(bullet);
		state->addGameObject(bullet);
	}
}

void Cannon::setBulletMesh(Mesh* mesh)
{
	bulletMesh = mesh;
}

void Cannon::setBulletMaterial(Material * mat)
{
	bulletMaterial = mat;
}

void Cannon::update(float dt)
{
	reloadTimer -= dt;

	for (auto bullet : bullets) {
		bullet->update( dt );
	}

	if (reloadTimer < 0.f) {
		shouldShoot = true;
	}
	if (shouldShoot) {
		shoot();
	}
}

void Cannon::shoot()
{
	// TODO: balance reload times, currently set to 3
	reloadTimer = reloadTime;
	shouldShoot = false;

	// TODO: get the direction to the player
	vec3 dirToPlayer = vec3(0.f, 1.f, 0.f);
	
	// TODO: balance speed of bullet, currently set to 0.5f
	for (auto bullet : bullets) {
		if (!bullet->active) {
			bullet->transform.position(transform.getComputed().position());
			bullet->setDir(dirToPlayer);
			bullet->transform.scale(vec3(1.f, 1.f, 1.f));
			bullet->rigidBody().solid(1);
			bullet->active = true;
			break;
		}
	}
}

void Cannon::handleCollision( ColliderObject* other, Manifold& m, double dt, size_t& numCollisions )
{
    numCollisions--;
}