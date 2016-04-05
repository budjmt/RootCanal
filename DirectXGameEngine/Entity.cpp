#include "Entity.h"
#include "Drawable.h"

Entity::Entity()
	: transform(_transform), active(_active)
{

}

Entity::Entity(Drawable* s)
	: transform(_transform), active(_active)
{
	_shape = s;
}

Entity::Entity(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s)
	: transform(_transform), active(_active)
{
	transform.position(p);
	transform.scale(sc);
	transform.rotate(r, rA);
	_shape = s;
}

Entity::Entity(const Entity& other)
	: transform(_transform), active(_active)
{
	transform = other.transform;
	_shape = other._shape;
	active = other.active;
}

Entity::~Entity(void) { delete _shape; }

EntityType Entity::type() { return _type; }

void Entity::update(float dt, Mouse* mouse) { }

void Entity::draw(ID3D11DeviceContext* deviceContext) {
	//glUniform4fv(shape->colorLoc, 1, &_color[0]);
	_shape->draw(&transform, deviceContext);
}