#pragma once

#include "ColliderObject.h"
#include "Ship.h"
#include "Bullet.h"

class Cannon :
	public ColliderObject
{
public:
	Cannon(Mesh* mesh, Material* material);

	void setBulletMesh(Mesh* mesh);
	void setBulletMaterial(Material* mat);

	void update(float dt);
	void shoot();
private:
	Ship* ship;

	Mesh* bulletMesh;
	Material* bulletMaterial;

	float reloadTimer;
	float reloadTime;
	bool shouldShoot;
};