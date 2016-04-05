#include "GameObject.h"

#include "DisplayEvent.h"

#include "Drawable.h"

GameObject::GameObject()
	: transform(_transform), active(_active)
{

}

GameObject::GameObject(Drawable* s)
	: transform(_transform), active(_active)
{
	_shape = s;
}

GameObject::GameObject(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s)
	: transform(_transform), active(_active)
{
	transform.position(p);
	transform.scale(sc);
	transform.rotate(r, rA);
	_shape = s;
}

GameObject::GameObject(const GameObject& other)
	: transform(_transform), active(_active)
{
	transform = other.transform;
	_shape = other._shape;
	active = other.active;
}

GameObject::~GameObject(void) { delete _shape; }

GameObjectType GameObject::type() { return _type; }

GameObject* GameObject::parent() const { return _parent; }
size_t GameObject::contains(GameObject* c) { return children.count(c); }

void GameObject::addChild(GameObject* c) { 
	if (!c) return;
	bool added = children.insert(c).second; 
	if (added) { 
		if (c->_parent)
			_parent->removeChild(c, false);
		// Dispatch event that child has been added
		DisplayEvent e(DisplayEvent::ADDED);
		c->dispatchEvent(e);
	}
	c->_parent = this;
}

void GameObject::removeChild(GameObject* c, bool del) {
	if (!c) return;
	size_t removed = children.erase(c);
	c->_parent = nullptr;
	if (removed) {
		// Dispatch event that child has been removed
		DisplayEvent e(DisplayEvent::REMOVED);
		c->dispatchEvent(e);
	}
	if (del)
		delete c;
}

void GameObject::update(float dt, Mouse* mouse) { }

void GameObject::draw(ID3D11DeviceContext* deviceContext) {
	//glUniform4fv(shape->colorLoc, 1, &_color[0]);
	_shape->draw(&transform, deviceContext);
}