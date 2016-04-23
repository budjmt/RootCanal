#include "UIAxes.h"



UIAxes::UIAxes() : UIObject()
{
	_shape = new DrawScreen();
}


UIAxes::~UIAxes()
{
	delete _shape;
}
