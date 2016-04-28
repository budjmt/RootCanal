#pragma once

#include "ColliderObject.h"
#include "Ship.h"
#include "Bullet.h"
#include "State.h"
#include <vector>
using namespace std;

class Cannon :
	public ColliderObject
{
public:
	Cannon(Mesh* mesh, Material* material, Mesh* bMesh, Material* bMaterial, State* gamestate, Ship* player);

	void setBulletMesh(Mesh* mesh);
	void setBulletMaterial(Material* mat);

	void update(float dt);
	void shoot();
private:
	Ship* ship;
	vector<Bullet*> bullets;

	State* state;
	Mesh* bulletMesh;
	Material* bulletMaterial;

	float reloadTimer;
	float reloadTime;
	bool shouldShoot;
};