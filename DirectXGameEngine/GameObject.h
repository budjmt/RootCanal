#pragma once

#include "DirectXGameCore.h"

#include "EventDispatcher.h"

#include "Mouse.h"
#include "Transform.h"

enum GameObjectType {
	NORMAL,
	COLLIDER,
	CAMERA,
	UI
};

class Drawable;

class GameObject
	: public EventDispatcher
{
public:
	GameObject();
	GameObject(Drawable* s);
	GameObject(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s);
	GameObject(const GameObject& other);
	GameObject(GameObject&& other) = default;
	GameObject& operator=(const GameObject& other) = default;
	GameObject& operator=(GameObject&& other) = default;
	virtual ~GameObject(void);

	Transform& transform;
	bool& active;
	GameObjectType type();

	GameObject* parent() const;
	std::unordered_set<GameObject*> children;
	size_t contains(GameObject* c);
	void addChild(GameObject* c);
	void removeChild(GameObject* c, bool del);

	virtual void update(float dt, Mouse* mouse);
	virtual void draw(ID3D11DeviceContext* deviceContext);

protected:
	GameObject* _parent;
    Drawable* _shape;
	Transform _transform;
	bool _active = true;
	GameObjectType _type;

	//friend void GameObject::addChild(GameObject* c);
	//friend void GameObject::removeChild(GameObject* c, bool del);
};

