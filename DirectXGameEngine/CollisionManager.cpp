#include "CollisionManager.h"

#include <iostream>
#include "DebugBenchmark.h"

CollisionManager::CollisionManager() { float d = 100.f; octTree = new OctTree(vec3(), vec3(d, d, d)); }
CollisionManager::~CollisionManager() { delete octTree; }

void CollisionManager::addObject(ColliderObject* o) {
    o->update( 0, nullptr );
	octTree->add(o);
}

void CollisionManager::update(float dt) {
    octTree->update();

	size_t numCollisions;
	do {
		collisionPairs = broadPhase();
		numCollisions = narrowPhase(dt);
	} while (numCollisions > 0);
}

void CollisionManager::draw() {
#if DEBUG
	octTree->draw();
#endif
}

void CollisionManager::clear()
{
    octTree->clear();
}

//returns a list of all pairs of colliders requiring narrow phase checks
collisionPairList CollisionManager::broadPhase() {
	return octTree->checkCollisions();
}

//returns the number of collisions found and handled
uint32_t CollisionManager::narrowPhase(float dt) {
	uint32_t numCollisions = 0;
	for (auto pair : collisionPairs) {
		//DebugBenchmark::start();
		auto *a = pair.first, *b = pair.second;
		if ( !( a->active && b->active ) || !( a->rigidBody().solid() && b->rigidBody().solid() ) )
			continue;

		Manifold m = a->collider()->intersects( b->collider() );
		if (m.originator) {
			if (m.originator == a->collider())
				a->handleCollision(b, m, dt, numCollisions);
			else 
				b->handleCollision(a, m, dt, numCollisions);
			a->collider()->update();
			b->collider()->update();
			++numCollisions;

			//std::cout << "collision! " << a->collider() << ", " << b->collider() << "; " << m.originator << ", " << m.pen << std::endl;
		}
		//std::cout << "Collision Check Time: " << DebugBenchmark::end() << std::endl;
	}
	return numCollisions;
}