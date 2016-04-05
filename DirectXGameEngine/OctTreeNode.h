#pragma once

#include <vector>
#include <iostream>

#include "Collider.h"
#include "ColliderObject.h"

typedef std::vector<std::pair<ColliderObject*, ColliderObject*>> collisionPairList;

class OctTreeNode
{
public:
    const size_t& CAPACITY = 8;

	OctTreeNode() = default;
	~OctTreeNode();
    OctTreeNode( vec3 center, vec3 halfWidths );

    void print();
    void draw();
    bool intersects( Collider* other );
    void add( ColliderObject* other );
    void branch();

    Collider* getCollider();
    size_t getCount();
    bool isLeaf();

    std::vector<ColliderObject*>& getContainedChildren();

    collisionPairList checkCollisions();
    collisionPairList checkCollisions( ColliderObject* other );

private:
	Transform transform;
    Collider* _collider;
    size_t _count; // How many colliders?
    std::vector<ColliderObject*> _containedChildren;
    bool _isLeaf;
    OctTreeNode* _nodes[8];
};