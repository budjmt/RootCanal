#include "RigidBody.h"

float RigidBody::floating() const { return _floating; };
void RigidBody::floating( float f ) { _floating = f; };
float RigidBody::fixed() const { return _fixed; };
void RigidBody::fixed( float s ) { _fixed = s; _floating = s; };
short RigidBody::solid() const { return _solid; };
void RigidBody::solid(short s) { _solid = s; };

float RigidBody::mass() const { return _mass; };
void RigidBody::mass( float m ) { _mass = m; _invMass = 1 / m; };
float RigidBody::invMass() const { return _invMass; };
float RigidBody::restitution() const { return _restitution; }
void RigidBody::restitution( float e ) { _restitution = e; }

vec3 RigidBody::vel() const { return _vel; };
void RigidBody::vel( vec3 v ) { _vel = v; _speed = vec3::length( v ); };
vec3 RigidBody::heading() const { return _heading; };

vec3 RigidBody::angVel() const { return _angVel; };
void RigidBody::angVel( vec3 a ) { _angVel = a; _angSpeed = vec3::length( a ); };
vec3 RigidBody::angHeading() const { return _angHeading; };

void RigidBody::update( double dt ) {
    updateVel( dt );
    updateAngVel( dt );

    netForce = vec3( 0, 0, 0 );
    netAngAccel = vec3( 0, 0, 0 );
}

#include <assert.h>
void RigidBody::updateVel( double dt ) {
    _vel += ( 1 - _fixed ) * _invMass * (float)dt * netForce;
    _speed = vec3::length( _vel );

    if( _speed < MIN_VEL ) {
        _vel = vec3( 0, 0, 0 );
        _speed = 0;
    }
    else {
        if( _speed > MAX_VEL ) {
            _vel *= MAX_VEL / _speed;
            _speed = MAX_VEL;
        }
        _heading = _vel / _speed;
    }
	//this is enough to determine if there's an issue
	assert(!NaN_CHECK(_speed));
}

void RigidBody::updateAngVel( double dt ) {
    _angVel += ( 1 - _fixed ) * (float)dt * netAngAccel;
    _angSpeed = vec3::length( _angVel );

    if( _angSpeed < MIN_VEL ) {
        _angVel = vec3( 0, 0, 0 );
        _angSpeed = 0;
    }
    else {
        if( _angSpeed > MAX_VEL ) {
            _angVel *= MAX_VEL / _angSpeed;
            _angSpeed = MAX_VEL;
        }
        _angHeading = _angVel / _angSpeed;
    }
}

//the coefficient here is equivalent to 0.5 * density of fluid (here just air) * C_d (drag coeff), which we boil down to C_d
//C_d is dependent on the object's shape and the Reynolds Number, R_e = internal forces / viscous forces = mag(v) * D / visc, 
//where D is some characteristic diameter or linear dimension and visc is the kinematic viscosity = viscosity / density
vec3 RigidBody::quadDrag( float c_d, vec3 v, vec3 h )
{
    return c_d * vec3::dot( v, v ) * h;
}