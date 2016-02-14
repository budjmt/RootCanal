#include "Transform.h"

Transform::Transform()
	: position(_position), scale(_scale), rotation(_rotation), rotAngle(_rotAngle), rotAxis(_rotAxis)
{
	parent = nullptr;
	_position = vec3(0, 0, 0);
	_scale = vec3(1, 1, 1);
	_rotation = quat();
	updateRot();
}

Transform::Transform(const Transform& other)
	: position(_position), scale(_scale), rotation(_rotation), rotAngle(_rotAngle), rotAxis(_rotAxis)
{
	parent = other.parent;
	_position = other.position;
	_scale = other.scale;
	_rotation = other.rotation;
	_rotAxis = other.rotAxis;
	_rotAngle = other.rotAngle;
	_forward = other.forward();
	_up = other.up();
	_right = other.right();
}


Transform::~Transform()
{
}

Transform& Transform::operator=(const Transform& other)
{
	_position = other.position;
	_scale = other.scale;
	_rotation = other.rotation;
	_rotAxis = other.rotAxis;
	_rotAngle = other.rotAngle;
	_forward = other.forward();
	_up = other.up();
	_right = other.right();
	return *this;
}

Transform Transform::computeTransform() {
	if (parent == nullptr) {
		this->updateRot();
		return *this;
	}
	Transform t = Transform();
	t.position = parent->position + _position;
	vec3 ps = parent->scale;
	t.scale = vec3(ps.x * _scale.x, ps.y * _scale.y, ps.z * _scale.z);
	t.rotation = _rotation * parent->rotation;
	t.parent = parent->parent;
	return t.computeTransform();
}

void Transform::updateNormals() {
	mat4 m = mat4::rotate(_rotAngle, _rotAxis);
	_forward = (vec3)(m * vec4(0, 0, 1, 1));
	_up = (vec3)(m * vec4(0, 1, 0, 1));
	_right = vec3::cross(_forward, _up);
}

vec3 Transform::forward() const { return _forward; }
vec3 Transform::up() const { return _up; }
vec3 Transform::right() const { return _right; }

void Transform::updateRot() {
	_rotAngle = _rotation.theta();
	_rotAxis = _rotation.axis();
	updateNormals();
}

//quats are rotated through the rotate function here
void Transform::rotate(vec3 v) { rotate(v.x, v.y, v.z); }
void Transform::rotate(float x, float y, float z) {
	if (x) _rotation = quat::rotate(_rotation, x, vec3(1, 0, 0));
	if (y) _rotation = quat::rotate(_rotation, y, vec3(0, 1, 0));
	if (z) _rotation = quat::rotate(_rotation, z, vec3(0, 0, 1));
	updateRot();
}

void Transform::rotate(float theta, vec3 axis) {
	if (theta) _rotation = quat::rotate(_rotation, theta, axis);
	updateRot();
}

vec3 Transform::getTransformed(vec3 v)
{
	Transform t = computeTransform();
	mat4 translate = mat4::translate(t.position);
	mat4 rot = mat4::rotate(t.rotAngle, t.rotAxis);
	mat4 scale = mat4::scale(t.scale);
	return (vec3)(translate * scale * rot * vec4(v, 1));
}