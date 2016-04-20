#include "Cannon.h"

Cannon::Cannon(Mesh * mesh, Material * material)
	:ColliderObject(mesh, material)
{

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
	reloadTimer += 3.0f;

	// TODO: get the direction to the player
	vec3 dirToPlayer = vec3(0.f, 0.f, 0.f);
	
	// TODO: balance speed of bullet, currently set to 0.5f
	Bullet* bullet = new Bullet(bulletMesh, bulletMaterial, 0.5f);
	// TODO: set the forward of the bullet to face the player using dirToPlayer
}