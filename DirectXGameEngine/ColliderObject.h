#pragma once
#include <memory>

#include "entity.h"
#include "RigidBody.h"
#include "Collider.h"
#include "DrawMesh.h"

class ColliderObject :
	public Entity
{
public:
	ColliderObject(Drawable* s);
	ColliderObject(vec3 p, vec3 dims, vec3 sc, vec3 rA, float r, Drawable* s);
	~ColliderObject();
	ColliderObject(const ColliderObject& other);
	ColliderObject& operator=(ColliderObject& other);

	RigidBody& rigidBody();
	Collider* collider() const; void collider(Collider* c);
	virtual void update(double dt);
	virtual void calcForces(double dt);
	void handleCollision(ColliderObject* other, Manifold& m, double dt);
	vec3 calcAngularAccel(Manifold& m, vec3 F);
private:
	Collider* _collider;
	RigidBody body;

	static std::vector<ColliderObject*> colliderEntities;
};