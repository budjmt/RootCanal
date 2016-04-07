#include "Player.h"

Player::Player(vec3 p, Drawable* s) {
	transform.position(p);
	transform.scale(vec3(.6f, .6f, .6f));
	transform.rotate(90.f, vec3(0.f, 1.f, 0.f));
	_shape = s;
}

Player::~Player() {

}

void Player::calcForces(double dt) {
	body.netForce += vec3(0, body.mass() * -9.8f * 0.5f * (1 - body.floating()), 0);//gravity
																					//collision resolution stuff here

	body.netForce += body.quadDrag(-0.15f, body.vel(), body.heading());//quadratic drag, no mass involved, it's all velocity dependent
	body.netAngAccel += body.quadDrag(-0.15f, body.angVel(), body.angHeading());//for ang accel too
	
	//body.netForce *= body.invMass();
}

void Player::update(double dt) {
	calcForces(dt);
	body.update(dt);
	keyboardInput(dt);
	transform.position(transform.position() + body.vel() * (float)dt);
	transform.rotate(body.angVel() * (float)dt);
}

void Player::keyboardInput(double dt) {
	Keyboard& keys = Keyboard::getInstance();

	if (keys.isDown('W')) {
		vec3 newPos = transform.position() + vec3(1.f, 0.f, 0.f) * (5.f * (float)dt);
		transform.position(newPos);
	}
	else if (keys.isDown('S')) {
		vec3 newPos = transform.position() + vec3(1.f, 0.f, 0.f) * (-5.f * (float)dt);
		transform.position(newPos);
	}
	if (keys.isDown('D')) {
		vec3 newPos = transform.position() + vec3(0.f, 0.f, 1.f) * (-5.f * (float)dt);
		transform.position(newPos);
	}
	else if (keys.isDown('A')) {
		vec3 newPos = transform.position() + vec3(0.f, 0.f, 1.f) * (5.f * (float)dt);
		transform.position(newPos);
	}
}


RigidBody& Player::rigidBody() { return body; }