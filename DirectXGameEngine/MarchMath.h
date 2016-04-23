#pragma once

#include <cmath>
#include <cstring>

//use for floating point error correction
#define EPS_CHECK(x) x < FLT_EPSILON && x > -FLT_EPSILON
//all NaN related comparisons evaluate to false, this could be implemented as x != x
//but that could be optimized out; std::isnan(x) is part of the standard library
#define NaN_CHECK(x) std::isnan(x)

const double PI_D = 3.14159265358979323846;
const float PI = (float)PI_D;

int sign( int i ); float signf( float f );
float maxf(float a, float b);
float minf(float a, float b);
float clampf(float val, float min, float max);
float lerpf( float a, float b, float t );

class vec4;
class mat3;
class mat4;
class vec3 {
public:
    vec3(); ~vec3(); vec3( const vec3& other ); vec3& operator=( const vec3& other );
    vec3& operator+=( const vec3& other ); vec3& operator-=( const vec3& other );
    vec3& operator*=( float f ); vec3& operator/=( float f );
    vec3( float _x, float _y, float _z ); vec3( float* _v ); vec3( vec4 v );
    float& x, &y, &z;
    bool operator==( const vec3& other ); bool operator<( const vec3& other );
    float operator[]( int i ) const;  float& operator[]( int i );
    vec3 operator+( const vec3& other ) const; vec3 operator-( const vec3& other ) const; vec3 operator-() const;
    vec3 operator*( float f ) const; vec3 operator/( float f ) const;
	vec3 operator*(const mat3& );

    static float length( const vec3 v );
    static float dot( const vec3 a, const vec3 b ); 
	static vec3 cross( const vec3 a, const vec3 b );
    static vec3 lerp( const vec3 a, const vec3 b, float t );
    static vec3 limit( vec3 v, float mag );

private:
    float v[3];
};
vec3 operator*( float f, const vec3& v ); vec3 operator/( float f, const vec3& v );

class vec4 {
public:
    vec4(); ~vec4(); vec4( const vec4& other ); vec4& operator=( const vec4& other );
    vec4( float _x, float _y, float _z, float _w ); vec4( float* _v ); vec4( vec3 _v, float _w = 1 );
    float& x, &y, &z, &w;
    float operator[]( int i ) const;  float& operator[]( int i );
    vec4 operator+( const vec4& other ); vec4 operator-( const vec4& other );
    vec4 operator*( float f ); vec4 operator/( float f );
	vec4 operator*(const mat4& m);

    static float length( vec4 v );
    static float dot( vec4 a, vec4 b );
    static vec4 lerp( vec4 a, vec4 b, float t );

private:
    float v[4];
};

class mat3 {
public:
	mat3(); ~mat3(); explicit mat3(float f); mat3(float r1, float r2, float r3); 
	mat3(vec3 c1, vec3 c2, vec3 c3); mat3(float* _m);
	const float* operator[](int i) const; float* operator[](int i);
	//column major
	mat3 operator+(const mat3& other); mat3 operator-(const mat3& other);
	mat3 operator*(float f); mat3 operator/(float f);
	mat3 operator*(const mat3& other);

	static mat3 transpose(mat3& m);

private:
	float m[9];
};

class mat4 {
public:
    mat4(); ~mat4(); explicit mat4( float f ); mat4(float r1, float r2, float r3, float r4); 
	mat4(vec4 c1, vec4 c2, vec4 c3, vec4 c4); mat4( float* _m );
    const float* operator[]( int i ) const; float* operator[]( int i );
    //column major
    mat4 operator+( const mat4& other ); mat4 operator-( const mat4& other );
    mat4 operator*( float f ); mat4 operator/( float f );
    mat4 operator*( const mat4& other );

    static mat4 transpose( mat4& m );
    //inverse transpose of a transformation matrix
    static mat4 inv_tp_tf( mat4& m );

    static mat4 translate( vec3 v );
    static mat4 rotate( float t, vec3 a );
    static mat4 scale( vec3 v );

    //all of these are pre-transposed
    static mat4 lookAt( vec3 pos, vec3 target, vec3 up );
    static mat4 perspectiveFOV( float fovy, float aspect, float zNear, float zFar );
    static mat4 perspective( float width, float height, float zNear, float zFar );
    static mat4 orthographic( float left, float right, float bottom, float top, float zNear, float zFar );
    static mat4 orthographic( float width, float height, float zNear, float zFar );

private:
    float m[16];
};

class quat {
public:
    quat();
    quat( float _x, float _y, float _z, float _w );
    quat( float v0, vec3 _v ); quat( vec3 a, float t );
    ~quat(); quat( const quat& other ); quat& operator=( const quat& other );
    //float& x, &y, &z, &w;
    float& w;
    vec3& v;
    float theta() const; void theta( float t );
    vec3 axis() const; void axis( vec3 a );

    quat operator+( const quat& other ); quat operator-( const quat& other );
    quat operator*( float f ); quat operator/( float f );
    quat operator*( const quat& other ) const;

	static float length(const quat& q);

    static quat pow( const quat& q, float e );
    static quat inverse( const quat& q );
    static quat rotate( const quat& q, float theta, vec3 axis );

    static quat slerp( const quat& a, const quat& b, float t );
    static quat rotation( float theta, vec3 axis );
    static quat point( float x, float y, float z );

private:
    float v0;
    vec3 _v;
    float _theta, sin_t_half; vec3 _axis;
};