#pragma once

#include <math.h>

float lerpf(float a, float b, float t);

class vec3 {
public:
	vec3(); ~vec3(); vec3(const vec3& other); vec3& operator=(const vec3& other);
	vec3(float _x, float _y, float _z); vec3(float* _v); vec3(vec4 v);
	float& x,& y,& z;
	float operator[](int i) const;  float& operator[](int i);
	vec3 operator+(const vec3& other); vec3 operator-(const vec3& other);
	vec3 operator*(float f); vec3 operator/(float f);

	static float length(vec3& v);
	static float dot(vec3& a, vec3& b); static vec3 cross(vec3& a, vec3& b);
	static vec3 lerp(vec3& a, vec3& b, float t);

private:
	float v[3];
};

class vec4 {
public:
	vec4(); ~vec4(); vec4(const vec4& other); vec4& operator=(const vec4& other);
	vec4(float _x, float _y, float _z, float _w); vec4(float* _v); vec4(vec3 _v, float _w = 1);
	float& x, &y, &z, &w;
	float operator[](int i) const;  float& operator[](int i);
	vec4 operator+(const vec4& other); vec4 operator-(const vec4& other);
	vec4 operator*(float f); vec4 operator/(float f);

	static float length(vec4& v);
	static float dot(vec4& a, vec4& b);
	static vec4 lerp(vec4& a, vec4& b, float t);

private:
	float v[4];
};

class mat4 {
public:
	mat4(); ~mat4(); mat4(float f); mat4(float* _m);
	const float* operator[](int i) const; float* operator[](int i);
	//column major
	mat4 operator+(const mat4& other); mat4 operator-(const mat4& other);
	mat4 operator*(float f); mat4 operator/(float f);
	mat4 operator*(const mat4& other);	vec4 operator*(const vec4& v);

	static mat4 transpose(mat4& m);

	static mat4 translate(vec3& v);
	static mat4 rotate(float t, vec3& a);
	static mat4 scale(vec3& v);

private:
	float m[16];
};

class quat {
public:
	quat();
	quat(float _x, float _y, float _z, float _w);
	quat(float v0, vec3 _v); quat(vec3 a, float t);
	~quat(); quat(const quat& other); quat& operator=(const quat& other);
	float& x, &y, &z, &w;
	vec3& v;
	float theta() const; void theta(float t);
	vec3 axis() const; void axis(vec3 a);

	quat operator+(const quat& other); quat operator-(const quat& other);
	quat operator*(float f); quat operator/(float f);
	quat operator*(const quat& other) const;

	static quat pow(const quat& q, int e);
	static quat inverse(const quat& q);
	static quat rotate(const quat& q, float theta, vec3 axis);

	static quat slerp(const quat& a, const quat& b, float t);
	static quat rotation(float theta, vec3 axis);
	static quat point(float x, float y, float z);

private:
	float v0;
	vec3 _v;
	float _theta, sin_t_half; vec3 _axis;
};