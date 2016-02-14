#pragma once

#include "MarchMath.h"

class Transform
{
public:
	Transform();
	Transform(const Transform& other);
	~Transform();
	Transform& operator=(const Transform& other);
	vec3& position, &scale;
	quat& rotation;	vec3& rotAxis; float& rotAngle;
	Transform* parent;
	Transform computeTransform();
	void updateNormals();
	vec3 forward() const;
	vec3 up() const;
	vec3 right() const;
	void updateRot();
	void rotate(float x, float y, float z);
	void rotate(vec3 v);
	void rotate(float theta, vec3 axis);

	vec3 getTransformed(vec3 v);
private:
	vec3 _position, _scale;
	quat _rotation; vec3 _rotAxis; float _rotAngle;
	vec3 _forward, _up, _right;
};


