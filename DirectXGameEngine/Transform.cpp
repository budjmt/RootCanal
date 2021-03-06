#include "Transform.h"

Transform::Transform()
{
    _parent = nullptr;
	computed = nullptr;
    _position = vec3();
    _scale = vec3( 1, 1, 1 );
    _rotation = quat();
    updateRot();
}

Transform::Transform( const Transform& other )
{
	//compute = &Transform::noCompute;
    parent(other._parent);
    _position = other._position;
    _scale = other._scale;
    _rotation = other._rotation;
    _rotAxis = other._rotAxis;
    _rotAngle = other._rotAngle;
	
	base_forward = other.base_forward;
	base_up = other.base_up;
	
	_forward = other._forward;
	_up = other._up;
	_right = other._right;

	if(other.computed) {
		computed = new Transform;
		*computed = *other.computed;
		//compute = &Transform::allocatedCompute;
	}
}


Transform::~Transform() {
	if(computed) delete computed;
}

Transform& Transform::operator=( const Transform& other )
{
	//compute = &Transform::noCompute;
	parent(other._parent);
    _position = other._position;
    _scale = other._scale;
    _rotation = other._rotation;
    _rotAxis = other._rotAxis;
    _rotAngle = other._rotAngle;
	
	base_forward = other.base_forward;
	base_up = other.base_up;
	
	_forward = other._forward;
	_up = other._up;
	_right = other._right;

	if (other.computed) {
        if( !computed )
            computed = new Transform;
		*computed = *other.computed;
		//compute = &Transform::allocatedCompute;
	}
    return *this;
}

void Transform::makeDirty() {
    dirty = true;
    for (auto child : children)
        child->dirty = true;
}
vec3& Transform::position() { return _position; } void Transform::position(vec3 v) { makeDirty(); _position = v; }
vec3& Transform::scale() { return _scale; } void Transform::scale(vec3 v) { makeDirty(); _scale = v; }
quat& Transform::rotation() { return _rotation; } void Transform::rotation(quat q) { makeDirty(); _rotation = q; }
vec3 Transform::rotAxis() const { return _rotAxis; } float Transform::rotAngle() const { return _rotAngle; }

Transform* Transform::parent() { return _parent; } 
void Transform::parent(Transform* p) {
	if (p == _parent)
		return;
	if (p) {
		p->children.insert(this);
		//if (!_parent)
			//compute = &Transform::firstCompute;
	}
	if (_parent) {
		_parent->children.erase(this);
		if (!p) {
			if (computed) delete computed;
			//compute = &Transform::noCompute;
		}
	}
	makeDirty();
	_parent = p; 
}

Transform Transform::getComputed() {
	//if there's no parent, this is already an accurate transform
	if (!_parent)
		return *this;
	//if there is no previously computed transform, allocate one so we can compute it
	else if (!computed)
		computed = new Transform;
	//if there is a previously computed transform and we haven't made any "dirtying" changes since computing it
	else if (!dirty)
		return *computed;
	//[re]compute the transform
	dirty = false;
	return computeTransform();
	//this doessssn't worrkk righggght now wwwhaaaat the fuck
	//return (this->*compute)();
}

Transform Transform::computeTransform() {
    if( !_parent ) {
        this->updateRot();
        return *this;
    }
    Transform t = Transform();
    t.position( _parent->position() + _position );
    vec3 ps = _parent->scale();
    t.scale( vec3( ps.x * _scale.x, ps.y * _scale.y, ps.z * _scale.z ) );
    t.rotation( _rotation * _parent->rotation() );
    t.parent( _parent->parent() );
	return *computed = t.computeTransform();
}

Transform Transform::noCompute() {
	//if there's no parent, this is already an accurate transform
	return *this;
}

Transform Transform::firstCompute() {
	//if there is no previously computed transform, allocate one so we can compute it
	computed = new Transform;
	//compute = &Transform::allocatedCompute;
	dirty = false;
	return computeTransform();
}

Transform Transform::allocatedCompute() {
	//if there is a previously computed transform and we haven't made any "dirtying" changes since computing it
	if (!dirty)
		return *computed;
	//[re]compute the transform
	dirty = false;
	return *computed = computeTransform();
}

void Transform::setBaseDirections(vec3 t_forward, vec3 t_up) {
	base_forward = t_forward;
	base_up = t_up;
	updateDirections();
}

void Transform::updateDirections() {
    mat4 m = mat4::rotate( _rotAngle, _rotAxis );
    _forward = (vec3)( vec4( base_forward ) * m );
    _up = (vec3)( vec4( base_up ) * m );
    _right = vec3::cross( _up, _forward );
}

vec3 Transform::forward() const { return _forward; }
vec3 Transform::up() const { return _up; }
vec3 Transform::right() const { return _right; }

void Transform::updateRot() {
    _rotAngle = _rotation.theta();
	_rotAxis = _rotation.axis();
    updateDirections();
}

//quats are rotated through the rotate function here
void Transform::rotate( vec3 v ) { rotate( v[0], v[1], v[2] ); }
void Transform::rotate( float x, float y, float z ) {
    if( x ) _rotation = quat::rotate( _rotation, x, vec3( 1, 0, 0 ) );
    if( y ) _rotation = quat::rotate( _rotation, y, vec3( 0, 1, 0 ) );
    if( z ) _rotation = quat::rotate( _rotation, z, vec3( 0, 0, 1 ) );
    updateRot();
}

void Transform::rotate( float theta, vec3 axis ) {
    if( theta ) _rotation = quat::rotate( _rotation, theta, axis );
    updateRot();
}

vec3 Transform::getTransformed( vec3 v )
{
    Transform t = getComputed();
    mat4 translate = mat4::translate( t.position() );
    mat4 rot = mat4::rotate( t.rotAngle(), t.rotAxis() );
    mat4 scale = mat4::scale( t.scale() );
	return (vec3)(vec4(v, 1) * scale * rot * translate);
}