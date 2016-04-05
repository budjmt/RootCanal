#pragma once

#include "DirectXGameCore.h"

#include "Mouse.h"
#include "Transform.h"

enum EntityType {
	NORMAL,
	COLLIDER,
	CAMERA
};

class Drawable;

class Entity
{
public:
	Entity();
	Entity(Drawable* s);
	Entity(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s);
	Entity(const Entity& other);
	Entity(Entity&& other) = default;
	Entity& operator=(const Entity& other) = default;
	Entity& operator=(Entity&& other) = default;
	virtual ~Entity(void);

	Transform& transform;
	bool& active;
	EntityType type();

	virtual void update(float dt, Mouse* mouse);
	virtual void draw(ID3D11DeviceContext* deviceContext);

protected:
    Drawable* _shape;
	Transform _transform;
	bool _active = true;
	EntityType _type;
};

