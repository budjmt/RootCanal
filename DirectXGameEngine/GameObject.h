#pragma once

#include <vector>
#include "MarchMath.h"

#include "EventDispatcher.h"
#include "DisplayEvent.h"
#include "Mesh.h"
#include "DrawMesh.h"

class GameObject :
    public EventDispatcher
{
public:
    GameObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
    virtual ~GameObject();

    virtual void update( float dt );
    virtual void draw( float dt );
    virtual void drawShape();

    virtual void addChild( GameObject* child );
    virtual void removeChild( GameObject* child, bool free = false );
    virtual bool contains( GameObject* child );

    //virtual void addForce( vec3 force );
    //virtual void addImpulse( vec3 impulse );

    GameObject* getParent();
    void setParent( GameObject* value );

    //virtual vec3 getPosition();
    //virtual void setPosition( vec3 value );
	
    //virtual vec3 getVelocity();
    //virtual void setVelocity( vec3 value );
	
    //virtual vec3 getAcceleration();
    //virtual void setAcceleration( vec3 value );
	
    //virtual vec3 getScale();
    //virtual void setScale( vec3 value );
	
    //virtual vec3 getRotationAxis();
    //virtual void setRotationAxis( vec3 value );
	
    //virtual vec3 getForward();
    //virtual void setForward( vec3 value );
	
    //virtual bool getSolid();
    //virtual void setSolid( bool value );
	
    //void rotate( float angle );

    //virtual float getRotation();
    //virtual void setRotation( float value );
	
    //virtual float getRotationalVelocity();
    //virtual void setRotationalVelocity( float value );
	
    //virtual float getRotationalAcceleration();
    //virtual void setRotationalAcceleration( float value );

    //virtual float getMass();
    //virtual void setMass( float value );

    //virtual float getBrightness();
    //virtual void setBrightness( float value );

    //virtual bool getFixed();
    //virtual void setFixed( bool value );

    virtual Mesh* getMesh();
    virtual void setMesh( Mesh* value );

    virtual bool collidesWith( GameObject* other );

    virtual std::string getGameObjType();
    virtual void setGameObjType( std::string gameObjType );

    //virtual bool getIsVisible();
    //virtual void setIsVisible( bool isVisible );

protected:
    GameObject* _parent;
    std::vector<GameObject*>* _children;
    //vec3 _position;
    //vec3 _velocity;
    //vec3 _acceleration;
    //vec3 _scale;
    //vec3 _rotationAxis;
    //vec3 _forward;
    //float _rotation;
    //float _rotationalVelocity;
    //float _rotationalAcceleration;
    //float _mass;
    //float _brightness;
    //bool _fixed;
    //bool _isSolid;
    bool _isVisible;
    DrawMesh* _drawMesh;
    std::string _gameObjType;
    //ID3D11Device* _device;
    //ID3D11DeviceContext* _deviceContext;
};