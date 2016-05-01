#pragma once
#include <memory>

#include "GameObject.h"

#include "RigidBody.h"
#include "Collider.h"

#include "DrawMesh.h"

class ColliderObject :
	public GameObject
{
public:
    ColliderObject( Mesh* mesh, Material* material );
	ColliderObject(Drawable* s);
	ColliderObject(vec3 p, vec3 dims, vec3 sc, vec3 rA, float r, Drawable* s);
	virtual ~ColliderObject();
	ColliderObject(const ColliderObject& other);
	ColliderObject& operator=(ColliderObject& other);

	RigidBody& rigidBody();
	Collider* collider() const; void collider(Collider* c);
	virtual void update( float dt, Mouse* mouse );
	virtual void calcForces(double dt);
	virtual void handleCollision(ColliderObject* other, Manifold& m, double dt, size_t& numCollisions);
	vec3 calcAngularAccel(Manifold& m, vec3 F);
protected:
	Collider* _collider;
	RigidBody body;

	static std::vector<ColliderObject*> colliderEntities;
};