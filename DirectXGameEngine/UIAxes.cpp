#include "UIAxes.h"

#include "Drawable.h"

UIAxes::UIAxes() : UIObject()
{
	_shape = new Drawable();
}


UIAxes::~UIAxes()
{
	delete _shape;
}
