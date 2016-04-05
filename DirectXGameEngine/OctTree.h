#pragma once

#include <iostream>
#include "MarchMath.h"
#include "OctTreeNode.h"

class OctTree
{
public:
    OctTree( vec3 center, vec3 halfWidths );
	OctTree() = default;
    ~OctTree();
	OctTree(const OctTree& other) = default;
	OctTree& operator=(const OctTree& other) = default;

    void print();
    void draw();
    bool intersects( Collider* other );
    collisionPairList checkCollisions();
	collisionPairList checkCollisions( ColliderObject* other );
    void add( ColliderObject* o );

    OctTreeNode* getRoot();

private:
    OctTreeNode* _root;
};
