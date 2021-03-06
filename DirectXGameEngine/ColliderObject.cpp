#include "ColliderObject.h"

#include "CollisionManager.h"

std::vector<ColliderObject*> ColliderObject::colliderEntities;

ColliderObject::ColliderObject( Mesh * mesh, Material * material )
    : ColliderObject(new DrawMesh(mesh,nullptr,DXInfo::getInstance().device))
{
    _shape->material( material );
}

ColliderObject::ColliderObject(Drawable* s)
	: GameObject(s)
{
	//_collider = new Collider(&transform,transform.scale);
	_collider = new Collider(((DrawMesh*)s)->mesh(), &transform);
	//CollisionManager::getInstance().addObject(this);
}

ColliderObject::ColliderObject(vec3 p, vec3 dims, vec3 sc, vec3 rA, float r, Drawable* s)
	: GameObject(p, sc, rA, r, s)
{
	//_collider = new Collider(&transform,dims);
	_collider = new Collider(((DrawMesh*)s)->mesh(), &transform);
	//CollisionManager::getInstance().addObject(this);
}

ColliderObject::~ColliderObject() {
	delete _collider;
}

ColliderObject::ColliderObject(const ColliderObject& other) : GameObject(other)
{
	collider(new Collider(*other.collider()));
	body = other.body;
}

ColliderObject& ColliderObject::operator=(ColliderObject& other) {
	collider(other.collider());
	other._collider = nullptr;
	body = other.body;
	return *this;
}

RigidBody& ColliderObject::rigidBody() { return body; }
Collider* ColliderObject::collider() const { return _collider; } void ColliderObject::collider(Collider* c) { _collider = c; }

void ColliderObject::update( float dt, Mouse* mouse ) {
	calcForces(dt);
	body.update(dt);
	transform.position( transform.position() + body.vel() * (float)dt );
	transform.rotate(body.angVel() * (float)dt);
	_collider->update();
	assert(!NaN_CHECK(transform.position().x));
}

void ColliderObject::calcForces(double dt) {
	body.netForce += vec3(0, body.mass() * -9.8f * 0.5f * (1 - body.floating()), 0);//gravity
	//collision resolution stuff here

	body.netForce += body.quadDrag(-0.5f, body.vel(), body.heading());//quadratic drag, no mass involved, it's all velocity dependent
	body.netAngAccel += body.quadDrag(-10.f, body.angVel(), body.angHeading());//for ang accel too

	//body.netForce *= body.invMass();
}

//override this (and preferably call it) to change on-collision behavior
//decrement the numCollisions counter if this collision is considered "resolved" without actually resolving the collision
void ColliderObject::handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions) {
	RigidBody oRB = other->rigidBody();
	float velAlongAxis = vec3::dot(oRB.vel() - body.vel(), m.axis);
	//if the two bodies are travelling in the same direction along the axis
	if (velAlongAxis > 0)
		return;

	//coefficient of restitution. we take the min of the two coeffs
	//when e = 0, it is a perfect inelastic/plastic collision, and the objects stick together
	//when 0 < e < 1, it is a regular inelastic collision, with some energy dissipated
	//when e = 1, it is an elastic collision, where all energy is put into the response
	float e = minf(body.restitution(), oRB.restitution());

	//j = magnitude of impulse
	float j = velAlongAxis;
	j *= -(1 + e);
	j /= body.invMass() + oRB.invMass();

	//vec3 impulse = j * m.axis;
	//float massRatio = mass / (mass + other->mass);
	//_vel -= massRatio * impulse;
	//massRatio *= other->mass * invMass;
	//other->vel(massRatio * impulse);

	//F is the force applied by the collision; we use the definition F = dp / dt, where p = momentum and dp = impulse
	j /= (float)dt;
	vec3 F = j * m.axis;
	body.netForce += F;
	oRB.netForce += -F;
	DrawDebug::getInstance().drawDebugVector(_transform.position(), _transform.position() + F);
	DrawDebug::getInstance().drawDebugVector(other->transform.position(), other->transform.position() - F);

	//they have the same collision points by definition, but vecs to those points change, meaning torque and covariance also change
	body.netAngAccel += calcAngularAccel(m, F);
	oRB.netAngAccel += other->calcAngularAccel(m, -F);

	//correct positions
	float percent = 1.2f, slop = 0.05f;
	vec3 correction = maxf(-m.pen - slop, 0.0f) * percent * (1 + body.fixed() + oRB.fixed()) / (body.invMass() + oRB.invMass()) * m.axis;
	transform.position( transform.position() - (body.invMass() + oRB.fixed() * oRB.invMass()) * (1 - body.fixed()) * correction );
	other->transform.position( other->transform.position() + (oRB.invMass() + body.fixed() * body.invMass()) * (1 - oRB.fixed()) * correction );

	assert(!NaN_CHECK(transform.position().x));
	assert(!NaN_CHECK(other->transform.position().x));
}

//Given a collision force F, calculates the change in angular acceleration it causes
vec3 ColliderObject::calcAngularAccel(Manifold& m, vec3 F) {
	vec3 torque = vec3();
	if (!m.colPoints.size() || body.fixed())
		return torque;

	mat3 C = mat3();//mass-weighted covariance

	//assumes uniform mass distribution; we can account for non-uniform distributions with constraints
	float m_n = body.mass() / m.colPoints.size();
	for (auto colPoint : m.colPoints) {
		vec3 r = colPoint - _collider->framePos();//vector from the center of mass to the collision point
		torque += vec3::cross(r, F);//torque = r x F = |r||F|sin(theta)
		C = C + mat3(r.x * r, r.y * r, r.z * r) * m_n;//m_n * r * r_transpose
	}
	float trace_C = C[0][0] + C[1][1] + C[2][2];

	mat3 iT = mat3(1) * trace_C - C;//inertia tensor = IdGameObject_3x3 * trace(C) - C

	vec3 at_iT = vec3(m.axis.x * (iT[0][0] + iT[0][1] + iT[0][2])
					, m.axis.y * (iT[1][0] + iT[1][1] + iT[1][2])
					, m.axis.z * (iT[2][0] + iT[2][1] + iT[2][2]));//axis_transpose * inertia tensor (matrices are column major)

	float inertia = vec3::dot(at_iT, m.axis);//axis_transpose * iT * axis = (axis_transpose * inertia tensor) . axis

	return (inertia) ? torque / inertia : vec3();
}