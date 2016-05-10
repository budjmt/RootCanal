#include "UIObject.h"

std::unordered_map<std::string, std::unique_ptr<DirectX::SpriteFont>> UIObject::fonts;

UIObject::UIObject() : GameObject() { _type = GameObjectType::UI; }
UIObject::UIObject(Drawable* s) : GameObject(s) { _type = GameObjectType::UI; }
UIObject::UIObject(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s) : GameObject(p,sc,rA,r,s) { _type = GameObjectType::UI; }

void UIObject::loadFonts() {
	fonts["arialblack"] = std::make_unique<DirectX::SpriteFont>(DXInfo::getInstance().device, L"../Assets/arialblack.spritefont");
}

void UIObject::update(float dt, Mouse* mouse) {

}

void UIObject::draw(ID3D11DeviceContext* deviceContext) {

}
