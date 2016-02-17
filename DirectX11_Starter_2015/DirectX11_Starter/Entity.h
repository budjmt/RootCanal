#pragma once

#include "Drawable.h"
#include "Transform.h"

enum EntityType {
	NORMAL,
	COLLIDER,
	CAMERA
};

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

	//virtual void update(double dt);
	//virtual void draw();
	void move(vec3 d);
	void update(float dt);
	void draw(ID3D11DeviceContext* deviceContext);
protected:
	Drawable* shape;
	vec4 _color;
	Transform _transform;
	bool _active = true;
	EntityType _type;
};