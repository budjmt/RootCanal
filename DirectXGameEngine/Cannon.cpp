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

	for (size_t i = 0, numBullets = bullets.size(); i < numBullets; i++) {
		bullets[i]->update( dt );
		if (!bullets[i]->active) {
			bullets[i] = bullets.back();
			bullets.pop_back();
			numBullets--;
		}
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
	Bullet* bullet = new Bullet(bulletMesh, bulletMaterial, 0.5f, dirToPlayer, ship);
	bullet->transform.position(vec3(0, 8, 0));

	bullets.push_back(bullet);
	state->addGameObject(bullet);
}
