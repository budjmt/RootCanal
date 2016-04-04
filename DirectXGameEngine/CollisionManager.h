#pragma once

#include "OctTree.h"

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void update();

	collisionPairList broadPhase();
	size_t narrowPhase();

private:
	OctTree octTree;
	collisionPairList collisionPairs;

};

