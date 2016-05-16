#include "OctTree.h"

OctTree::OctTree( vec3 center, vec3 halfWidths )
{
    _center = center;
    _halfWidths = halfWidths;

    _root = new OctTreeNode( center, halfWidths );
}

OctTree::~OctTree()
{
    if(_root) { delete _root; }
}

void OctTree::print()
{
    std::cout << "Collider Binary Tree: " << std::endl;
    _root->print();
}

void OctTree::draw()
{
#if DEBUG
	_root->draw();
#endif
}

bool OctTree::intersects( Collider* other )
{
    return _root->intersects( other );
}

void OctTree::add(ColliderObject* o) { _root->add(o); }

void OctTree::clear()
{
    if( _root ) { delete _root; }
    _root = new OctTreeNode( _center, _halfWidths );
}

void OctTree::update()
{
    if (_root)
        _root->update();
}

OctTreeNode* OctTree::getRoot() { return _root; }

collisionPairList OctTree::checkCollisions() { return _root->checkCollisions(); }
collisionPairList OctTree::checkCollisions( ColliderObject* other ) { return _root->checkCollisions( other ); }