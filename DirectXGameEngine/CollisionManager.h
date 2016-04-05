#pragma once

#include "OctTree.h"
#include "ColliderObject.h"

class CollisionManager
{
public:
	static CollisionManager& getInstance() { static CollisionManager instance; return instance; }

	void addObject(ColliderObject* o);
	void update(float dt);

	collisionPairList broadPhase();
	uint32_t narrowPhase(float dt);

private:
	CollisionManager();
	~CollisionManager();

	OctTree octTree;
	collisionPairList collisionPairs;

};

