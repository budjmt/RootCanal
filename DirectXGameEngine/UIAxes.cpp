#include "UIAxes.h"

#include "Vertex.h"
#include "Drawable.h"

UIAxes::UIAxes() : UIObject()
{
	_shape = new Drawable();

	UINT stride = sizeof(DebugVector);
	//_shape->vertexBuffer = 
}


UIAxes::~UIAxes()
{
	delete _shape;
}

void UIAxes::update(float dt, Mouse* mouse) {
	//_transform.rotation((*camera)->transform().rotation());
}

void UIAxes::draw(ID3D11DeviceContext* deviceContext) {
	
}