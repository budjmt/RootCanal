#include "CollisionManager.h"

CollisionManager::CollisionManager() { octTree = OctTree(vec3(), vec3(1000,1000,1000)); }


CollisionManager::~CollisionManager() { }

void CollisionManager::addObject(GameObject* go) {
	octTree.add(go);
}

void CollisionManager::update() {
	size_t numCollisions;
	do {
		collisionPairs = broadPhase();
		numCollisions = narrowPhase();
	} while (numCollisions > 0);
}

//returns a list of all pairs of colliders requiring narrow phase checks
collisionPairList CollisionManager::broadPhase() {
	return octTree.checkCollisions();
}

//returns the number of collisions found and handled
size_t CollisionManager::narrowPhase() {
	size_t numCollisions = 0;
	for (auto pair : collisionPairs) {
		//check for and handle collision
	}
	return numCollisions;
}