#include "OctTreeNode.h"

OctTreeNode::OctTreeNode( vec3 center, vec3 halfWidths )
{
	transform.position( center );
	_collider = new Collider(&transform, halfWidths);

    //AABB& nodeBox = _collider->aabb();
    //nodeBox.center = center;
    //nodeBox.halfDims = halfWidths;

    _containedChildren = std::vector<GameObject*>( CAPACITY );
    _nodes = std::vector<OctTreeNode*>( CAPACITY );

    _count = 0;
    _isLeaf = true;

    for( size_t i = 0; i < CAPACITY; i++ )
        _containedChildren[i] = nullptr;

    for( size_t i = 0; i < _nodes.size(); i++ )
        _nodes[i] = nullptr;
}

OctTreeNode::~OctTreeNode()
{
    for( size_t i = 0, numNodes = _nodes.size(); i < numNodes; i++ )
        if( _nodes[i] )
            delete _nodes[i];

    delete _collider;
}

void OctTreeNode::print()
{
    std::cout << std::fixed << std::setprecision( 2 ) << "OctTreeNode: " << std::endl;
    std::cout << "Bounding Box:" << std::endl;
    //_collider->aabb()->print();

    std::cout << std::endl;

    std::cout << "Colliders: " << _count << std::endl;

	for each(GameObject* child in _containedChildren)
		if (child)
			std::cout << child->collider() << std::endl;
            //child->collider().aabb()->print();

    std::cout << std::endl;

    std::cout << "Nodes: " << std::endl;
    std::string nodeNames[8] =
    {
        "Front Top Left Node",		"Front Top Right Node",
        "Front Bottom Left Node",	"Front Bottom Right Node",
        "Back Top Left Node",		"Back Top Right Node",
        "Back Bottom Left Node",	"Back Bottom Right Node"
    };

    for( size_t i = 0, numNodes = _nodes.size(); i < numNodes; i++ )
    {
        OctTreeNode* child = _nodes[i];

        if(!child)
            std::cout << nodeNames[i].c_str() << " = nullptr" << std::endl;
        else
        {
            std::cout << nodeNames[i].c_str() << ":" << std::endl;
            child->print();
        }
    }

    std::cout << std::endl;
}

void OctTreeNode::draw()
{
#if DEBUG
	auto d = DrawDebug::getInstance();
	AABB a = _collider->aabb();
	d.drawBox(a.center, a.halfDims, vec4(0,0,0,1), false);

    for each( GameObject* child in _containedChildren )
    {
        if( child )
        {
			AABB aabb = child->collider()->aabb();
			d.drawBox(aabb.center, aabb.halfDims, vec4(0,0,0,1), false);
        }
    }

    for each( OctTreeNode* child in _nodes )
        if( child )
            child->draw();
#endif
}

bool OctTreeNode::intersects( Collider* other )
{
    // Filter out colliders that don't hit the bounding box
    if( !_collider->aabb().intersects( other->aabb() ) )
        return false;

    // Check collisions vs each collider (leaf)
    if( _isLeaf )
    {
        for each( GameObject* child in _containedChildren )
			if( child && child->collider().aabb().intersects( other->aabb() ) )
			   return true;
    }

    // Check collisions with children (not a leaf)
    else
    {
        for( size_t i = 0; i < _nodes.size(); i++ )
            if( _nodes[i]->intersects( other ) )
                return true;
    }

    return false;
}

collisionPairList OctTreeNode::checkCollisions()
{
	collisionPairList collisionPairs;
    if( _isLeaf )
    {
        for( size_t i = 0, numChildren = _containedChildren.size(); i < numChildren; i++ )
        {
            if( !_containedChildren[i] )
                continue;

            for( size_t j = i + 1; j < numChildren; j++ )
            {
                if( !_containedChildren[j] )
                    continue;

                if( _containedChildren[i]->collider().aabb().intersects( _containedChildren[j]->collider().aabb() ) )
                {
					//instead of doing this directly, add the pair of gameobjects
					//to a narrow phase check vector to be processed later
                    collisionPairs.push_back( std::pair<GameObject*, GameObject*>( _containedChildren[i], _containedChildren[j] ) );
                }
            }
        }
    }
    else
    {
		for (size_t i = 0, numNodes = _nodes.size(); i < numNodes; i++) {
			auto cp = _nodes[i]->checkCollisions();
			if (cp.size() > 0)
				collisionPairs.insert( collisionPairs.end(), cp.begin(), cp.end() );
		}
    }

	return collisionPairs;
}

collisionPairList OctTreeNode::checkCollisions( GameObject* other )
{
	collisionPairList collisionPairs;
    if( _isLeaf )
    {
        for( size_t i = 0, numChildren = _containedChildren.size(); i < numChildren; i++ )
        {
            if( !_containedChildren[i] || _containedChildren[i] == other )
                continue;

            if( _containedChildren[i]->collider().aabb().intersects( other->collider().aabb() ) )
            {
				collisionPairs.push_back(std::pair<GameObject*, GameObject*>(_containedChildren[i], other));
            }
        }
    }
    else
    {
        for( size_t i = 0; i < _nodes.size(); i++ )
        {
            Collider* nodeGameObject = _nodes[i]->getCollider();

            if( nodeGameObject->aabb().intersects( other->collider().aabb() ) )
            {
				auto cp = _nodes[i]->checkCollisions(other);
				if (cp.size() > 0)
					collisionPairs.insert( collisionPairs.end(), cp.begin(), cp.end() );
            }
        }
    }

	return collisionPairs;
}

void OctTreeNode::add( GameObject* other )
{
    // Filter out colliders that don't hit binary box
    if( !_collider->aabb().intersects( other->collider().aabb() ) )
        return;

    // Branch if needed
    if( _count + 1 > CAPACITY )
        branch();

    // Add collider to this node (leaf) and increment count
    if( _isLeaf )
    {
        _containedChildren[_count] = other;
        _count++;
    }

    // Or add collider to its children (not a leaf)
    else
    {
        for( size_t i = 0, numNodes = _nodes.size(); i < numNodes; i++ )
            _nodes[i]->add( other );
    }
}

void OctTreeNode::branch()
{
    AABB& nodeBox = _collider->aabb();

    // Set leaf = false
    _isLeaf = false;

    // Make child nodes
    float quarterWidthX = nodeBox.halfDims.x * 0.5f;
    float quarterWidthY = nodeBox.halfDims.y * 0.5f;
    float quarterWidthZ = nodeBox.halfDims.z * 0.5f;
    vec3 center = nodeBox.center;
    vec3 quarterWidths = vec3( quarterWidthX, quarterWidthY, quarterWidthZ );

    setFrontTopLeftNode(		new OctTreeNode( center + vec3( -quarterWidthX,  quarterWidthY,  quarterWidthZ ), quarterWidths ) );
    setFrontTopRightNode(		new OctTreeNode( center + vec3(  quarterWidthX,  quarterWidthY,  quarterWidthZ ), quarterWidths ) );
    setFrontBottomLeftNode(		new OctTreeNode( center + vec3( -quarterWidthX, -quarterWidthY,  quarterWidthZ ), quarterWidths ) );
    setFrontBottomRightNode(	new OctTreeNode( center + vec3(  quarterWidthX, -quarterWidthY,  quarterWidthZ ), quarterWidths ) );
    setBackTopLeftNode(			new OctTreeNode( center + vec3( -quarterWidthX,  quarterWidthY, -quarterWidthZ ), quarterWidths ) );
    setBackTopRightNode(		new OctTreeNode( center + vec3(  quarterWidthX,  quarterWidthY, -quarterWidthZ ), quarterWidths ) );
    setBackBottomLeftNode(		new OctTreeNode( center + vec3( -quarterWidthX, -quarterWidthY, -quarterWidthZ ), quarterWidths ) );
    setBackBottomRightNode(		new OctTreeNode( center + vec3(  quarterWidthX, -quarterWidthY, -quarterWidthZ ), quarterWidths ) );

    // Add this node's colliders to the children
    // Set this node's contained children to nullptr
    for( size_t i = 0, numNodes = _nodes.size(); i < _count; i++ )
        for( size_t j = 0; j < numNodes; j++ )
            _nodes[j]->add( _containedChildren[i] );

    for( size_t i = 0; i < _count; i++ )
        _containedChildren[i] = nullptr;

    // Set count = 0
    _count = 0;
}

Collider* OctTreeNode::getCollider() { return _collider; }
size_t OctTreeNode::getCount() { return _count; }
bool OctTreeNode::isLeaf() { return _isLeaf; }

std::vector<GameObject*>& OctTreeNode::getContainedChildren() { return _containedChildren; }

OctTreeNode* OctTreeNode::getFrontTopLeftNode() { return _nodes[0]; }
void OctTreeNode::setFrontTopLeftNode( OctTreeNode* value ) { _nodes[0] = value; }
OctTreeNode* OctTreeNode::getFrontTopRightNode() { return _nodes[1]; }
void OctTreeNode::setFrontTopRightNode( OctTreeNode* value ) { _nodes[1] = value; }

OctTreeNode* OctTreeNode::getFrontBottomLeftNode() { return _nodes[2]; }
void OctTreeNode::setFrontBottomLeftNode( OctTreeNode* value ) { _nodes[2] = value; }
OctTreeNode* OctTreeNode::getFrontBottomRightNode() { return _nodes[3]; }
void OctTreeNode::setFrontBottomRightNode( OctTreeNode* value ) { _nodes[3] = value; }

OctTreeNode* OctTreeNode::getBackTopLeftNode() { return _nodes[4]; }
void OctTreeNode::setBackTopLeftNode( OctTreeNode* value ) { _nodes[4] = value; }
OctTreeNode* OctTreeNode::getBackTopRightNode() { return _nodes[5]; }
void OctTreeNode::setBackTopRightNode( OctTreeNode* value ) { _nodes[5] = value; }

OctTreeNode* OctTreeNode::getBackBottomLeftNode() { return _nodes[6]; }
void OctTreeNode::setBackBottomLeftNode( OctTreeNode* value ) { _nodes[6] = value; }
OctTreeNode* OctTreeNode::getBackBottomRightNode() { return _nodes[7]; }
void OctTreeNode::setBackBottomRightNode( OctTreeNode* value ) { _nodes[7] = value; }