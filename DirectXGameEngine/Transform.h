#pragma once

#include <unordered_set>

#include "MarchMath.h"

class Transform
{
public:
    Transform();
    Transform( const Transform& other );
	Transform( Transform&& other) = default;
    ~Transform();
    Transform& operator=( const Transform& other );
	Transform& operator=(Transform&& other) = default;

	bool dirty;
	void makeDirty();
	vec3& position(); void position(vec3 v);
	vec3& scale(); void scale(vec3 v);
    quat& rotation(); void rotation(quat q);
	vec3 rotAxis() const; float rotAngle() const;
    
	Transform* parent(); void parent(Transform* p);
	std::unordered_set<Transform*> children;
	Transform getComputed();
	
    void updateNormals();
    vec3 forward() const;
    vec3 up() const;
    vec3 right() const;
    void updateRot();
    void rotate( float x, float y, float z );
    void rotate( vec3 v );
    void rotate( float theta, vec3 axis );

    vec3 getTransformed( vec3 v );
private:
    vec3 _position, _scale;
    quat _rotation; vec3 _rotAxis; float _rotAngle;
    vec3 _forward, _up, _right;

	Transform *computed = nullptr, *_parent = nullptr;
	Transform computeTransform();
	
	//typedef Transform(Transform::*ComputeTransformFunc)();
	Transform noCompute();
	Transform firstCompute();
	Transform allocatedCompute();
	//ComputeTransformFunc compute = &Transform::noCompute;
};