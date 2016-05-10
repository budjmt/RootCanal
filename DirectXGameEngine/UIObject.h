#pragma once

#include <unordered_map>

#include "SpriteBatch.h"
#include "SpriteFont.h"

#include "DXInfo.h"
#include "GameObject.h"

//UIObjects are GameObjects that have screen-space transforms
//Their representation is highly individualized; some are textures, some have geometry etc.
//As such, it's expected that child classes will set up their buffers themselves
class UIObject : public GameObject
{
public:
	UIObject(); virtual ~UIObject() {};
	UIObject(Drawable* s);
	UIObject(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s);

	static void UIObject::loadFonts();

	virtual void update(float dt, Mouse* mouse) = 0;
	virtual void draw(ID3D11DeviceContext* deviceContext);

	static std::unordered_map<std::string, std::unique_ptr<DirectX::SpriteFont>> fonts;
};

