#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include "GameObject.h"
#include "Collider.h"

typedef std::vector<std::pair<GameObject*, GameObject*>> collisionPairList;

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
    void add( GameObject* other );
    void branch();

    Collider* getCollider();
    size_t getCount();
    bool isLeaf();

    std::vector<GameObject*>& getContainedChildren();

    OctTreeNode* getFrontTopLeftNode();
    void setFrontTopLeftNode( OctTreeNode* value );

    OctTreeNode* getFrontTopRightNode();
    void setFrontTopRightNode( OctTreeNode* value );

    OctTreeNode* getFrontBottomLeftNode();
    void setFrontBottomLeftNode( OctTreeNode* value );

    OctTreeNode* getFrontBottomRightNode();
    void setFrontBottomRightNode( OctTreeNode* value );

    OctTreeNode* getBackTopLeftNode();
    void setBackTopLeftNode( OctTreeNode* value );

    OctTreeNode* getBackTopRightNode();
    void setBackTopRightNode( OctTreeNode* value );

    OctTreeNode* getBackBottomLeftNode();
    void setBackBottomLeftNode( OctTreeNode* value );

    OctTreeNode* getBackBottomRightNode();
    void setBackBottomRightNode( OctTreeNode* value );

    collisionPairList checkCollisions();
    collisionPairList checkCollisions( GameObject* other );

private:
	Transform transform;
    Collider* _collider;
    size_t _count; // How many colliders?
    std::vector<GameObject*> _containedChildren;
    bool _isLeaf;
    std::vector<OctTreeNode*> _nodes;
};