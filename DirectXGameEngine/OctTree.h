#pragma once

#include <iostream>
#include "MarchMath.h"
#include "OctTreeNode.h"
#include "GameObject.h"

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
    bool intersects( Collider* otherColliderPtr );
    collisionPairList checkCollisions();
	collisionPairList checkCollisions( GameObject* other );
    void add( GameObject* otherColliderPtr );

    OctTreeNode* getRoot();

private:
    OctTreeNode* _root;
};
