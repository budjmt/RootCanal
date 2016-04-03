#pragma once

#include "MarchMath.h"

const float MIN_VEL = 1.f;//it's strangely high
const float MAX_VEL = 40.f;

class RigidBody {
public:
    vec3 netForce = vec3( 0, 0, 0 ), netAngAccel = vec3( 0, 0, 0 );

    short floatingObj() const; void floatingObj( short f );
    short staticObj() const; void staticObj( short s );

    float mass() const; void mass( float m );
    float invMass() const;
    float restitution() const; void restitution( float e );

    vec3 vel() const; void vel( vec3 v );
    vec3 heading() const;

    vec3 angVel() const; void angVel( vec3 a );
    vec3 angHeading() const;

    void update( double dt );
    void updateVel( double dt );
    void updateAngVel( double dt );

    vec3 quadDrag( float c_d, vec3 v, vec3 h );

private:
    short _floatingObj = 0
        , _staticObj = 0;
    float _mass = 1, _invMass = 1;
    float _speed = 0, _angSpeed = 0;
    float _restitution = 1;
    vec3 _vel = vec3( 0, 0, 0 ), _heading = vec3( 1, 0, 0 )
        , _angVel = vec3( 0, 0, 0 ), _angHeading = vec3( 1, 0, 0 );
};