#pragma once
#include <memory>
#include "GameObject.h"
#include "RigidBody.h"
#include "DrawMesh.h"
#include "Keyboard.h"

class Player :
	public GameObject
{
public:
	Player(vec3 p, Drawable* s);
	~Player();
	virtual void update(double dt);
	virtual void calcForces(double dt);
	virtual void keyboardInput(double dt);

	RigidBody& rigidBody();
private:
	RigidBody body;
};