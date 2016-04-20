#include "UIObject.h"

UIObject::UIObject() { _type = GameObjectType::UI; }
UIObject::UIObject(Drawable* s) : GameObject(s) { _type = GameObjectType::UI; }
UIObject::UIObject(vec3 p, vec3 sc, vec3 rA, float r, Drawable* s) : GameObject(p,sc,rA,r,s) { _type = GameObjectType::UI; }

void UIObject::update(float dt, Mouse* mouse) {

}

void UIObject::draw(ID3D11DeviceContext* deviceContext) {

}
