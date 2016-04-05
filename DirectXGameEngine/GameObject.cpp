#include "GameObject.h"

GameObject::GameObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext ) :
    EventDispatcher()
{
    _parent = nullptr;
    _children = new std::vector<GameObject*>();
    //_position = vec3( 0, 0, 0 );
    //_velocity = vec3( 0, 0, 0 );
    //_acceleration = vec3( 0, 0, 0 );
    //_scale = vec3( 1, 1, 1 );
    //_rotationAxis = vec3( 0, 0, 1 );
    //_forward = vec3( 0, 0, 1 );
    //_rotation = 0;
    //_rotationalVelocity = 0;
    //_rotationalAcceleration = 0;
    //_mass = 1.0f;
    //_brightness = 0.2f;
    //_fixed = false;
    //_drawMesh = nullptr;
    //_isSolid = false;
    //_isVisible = true;
    //_device = device;
    //_deviceContext = deviceContext;
}

GameObject::~GameObject()
{
    // TDODO Notify all children that they were removed
    /*auto begin = _children->begin();
    auto end = _children->end();
    for( std::vector<GameObject*>::iterator it = begin; it != end; ++it )
    {
    // Dispatch event that child has been removed
    GameObject* curChild = *it;
    Event e( Event::REMOVED );
    curChild->dispatchEvent( e );
    }*/

    /*auto begin = _children->begin();
    auto end = _children->end();
    for( std::vector<GameObject*>::iterator it = begin; it != end; ++it )
    {
    // Free child memory here? Or should it be removed where it's created?
    //delete *it;
    }*/

    delete _children;
}

void GameObject::update( float dt )
{
    /*if( !_fixed )
    {
        // Apply "Euler Integration" for physics
        _velocity += _acceleration * dt / _mass;
        setPosition( _position + _velocity * dt );

        _rotationalVelocity += _rotationalAcceleration * dt / _mass;
        setRotation( _rotation + _rotationalVelocity );
    }*/

    // Update all nested children
    auto begin = _children->begin();
    auto end = _children->end();
    for( std::vector<GameObject*>::iterator it = begin; it != end; ++it )
    {
        ( *it )->update( dt );
    }
}

void GameObject::draw( float dt )
{
    if( _isVisible )
    {
        drawShape();

        // Draw all nested children
        auto begin = _children->begin();
        auto end = _children->end();
        for( std::vector<GameObject*>::iterator it = begin; it != end; ++it )
        {
            ( *it )->draw( dt );
        }
    }
}

void GameObject::drawShape()
{
    if( _drawMesh != nullptr )
    {
       /* mat4 worldMatrix =
            mat4::translate( _position ) *
            mat4::rotate( _rotation, _rotationAxis ) *
            mat4::scale( _scale );*/

        // TODO: Fix all uniform setting stuff
        //setShaderFloat( _programIndex, "brightness", _brightness );
        //setShaderMatrix( _programIndex, "worldMatrix", worldMatrix );

        /*_drawMesh->draw(
            _position.x,
            _position.y,
            _position.z,
            _scale.x,
            _scale.y,
            _scale.z,
            _deviceContext
        );*/

        // Reset world matrix for any upcoming draw calls
        //setShaderMatrix( _programIndex, "worldMatrix", scale( vec3( 1, 1, 1 ) ) );
        // _boundingBox->draw();
    }
}

void GameObject::addChild( GameObject* child )
{
    if( !contains( child ) )
    {
        _children->push_back( child );

        // Dispatch event that child has been added
        DisplayEvent e( DisplayEvent::ADDED );
        child->dispatchEvent( e );
        child->setParent( this );
    }
}

void GameObject::removeChild( GameObject* child, bool free )
{
    auto begin = _children->begin();
    auto end = _children->end();
    for( std::vector<GameObject*>::iterator it = begin; it != end; ++it )
    {
        GameObject* curChild = *it;

        if( curChild == child )
        {
            // Dispatch event that child has been removed
            DisplayEvent e( DisplayEvent::REMOVED );
            child->dispatchEvent( e );
            child->setParent( nullptr );

            _children->erase( it );

            if( free )
            {
                delete curChild;
            }

            break;
        }
    }
}

bool GameObject::contains( GameObject* child )
{
    auto begin = _children->begin();
    auto end = _children->end();
    for( auto it = begin; it != end; ++it )
    {
        GameObject* curChild = *it;

        if( curChild == child )
            return true;
    }

    return false;
}

/*
void GameObject::addForce( vec3 force )
{
    _acceleration += force / _mass;
}

void GameObject::addImpulse( vec3 impulse )
{
    _velocity += impulse;
}
*/

GameObject* GameObject::getParent()
{
    return _parent;
}
void GameObject::setParent( GameObject* value )
{
    _parent = value;
}
/*
vec3 GameObject::getPosition()
{
    return _position;
}
void GameObject::setPosition( vec3 value )
{
    _position = value;
}

vec3 GameObject::getVelocity()
{
    return _velocity;
}
void GameObject::setVelocity( vec3 value )
{
    _velocity = value;
}

vec3 GameObject::getAcceleration()
{
    return _acceleration;
}
void GameObject::setAcceleration( vec3 value )
{
    _acceleration = value;
}

vec3 GameObject::getScale()
{
    return _scale;
}
void GameObject::setScale( vec3 value )
{
    _scale = value;
}

vec3 GameObject::getRotationAxis()
{
    return _rotationAxis;
}
void GameObject::setRotationAxis( vec3 value )
{
    _rotationAxis = value;
}

vec3 GameObject::getForward()
{
    return _forward;
}
void GameObject::setForward( vec3 value )
{
    _forward = value;
}

void GameObject::rotate( float angle )
{
    setRotation( _rotation + angle );
}

float GameObject::getRotation()
{
    return _rotation;
}
void GameObject::setRotation( float value )
{
    _rotation = value;
}

float GameObject::getRotationalVelocity()
{
    return _rotationalVelocity;
}
void GameObject::setRotationalVelocity( float value )
{
    _rotationalVelocity = value;
}

float GameObject::getRotationalAcceleration()
{
    return _rotationalAcceleration;
}
void GameObject::setRotationalAcceleration( float value )
{
    _rotationalAcceleration = value;
}

float GameObject::getMass()
{
    return _mass;
}
void GameObject::setMass( float value )
{
    _mass = value;
}

float GameObject::getBrightness()
{
    return _brightness;
}

void GameObject::setBrightness( float value )
{
    _brightness = value;
}

bool GameObject::getFixed()
{
    return _fixed;
}
void GameObject::setFixed( bool value )
{
    _fixed = value;
}

bool GameObject::getSolid()
{
    return _isSolid;
}
void GameObject::setSolid( bool value )
{
    _isSolid = value;
}
*/
Mesh* GameObject::getMesh()
{
    if( _drawMesh )
    {
        return _drawMesh->mesh();
    }

    return nullptr;
}
void GameObject::setMesh( Mesh* value )
{
    if( _drawMesh )
    {
        delete _drawMesh;
    }

    //_drawMesh = new DrawMesh( value, nullptr, _device );
}

bool GameObject::collidesWith( GameObject* other )
{
    // @TODO
    return false;
}

std::string GameObject::getGameObjType()
{
    return _gameObjType;
}

void GameObject::setGameObjType( std::string gameObjType )
{
    _gameObjType = gameObjType;
}

/*bool GameObject::getIsVisible()
{
    return _isVisible;
}

void GameObject::setIsVisible( bool isVisible )
{
    _isVisible = isVisible;
}*/