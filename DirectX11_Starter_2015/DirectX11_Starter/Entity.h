#pragma once

#include "DirectXGameCore.h"

//#include "Drawable.h"
#include "Transform.h"
#include "Mouse.h"

enum EntityType {
	NORMAL,
	COLLIDER,
	CAMERA
};

class Drawable;

class Entity {
public:
	Entity();
	Entity(Drawable* s);
	Entity(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s);
	Entity(const Entity& other);
	~Entity(void);

	Transform& transform;
	bool& active;
	vec4 color() const; void color(vec4& c);
	EntityType type();

	void move(vec3 d);
	virtual void update(float dt, Mouse* mouse);
	virtual void draw(ID3D11DeviceContext* deviceContext);
protected:
	Drawable* shape;
	vec4 _color;
	Transform _transform;
	bool _active = true;
	EntityType _type;
};