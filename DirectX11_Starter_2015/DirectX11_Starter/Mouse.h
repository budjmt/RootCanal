#pragma once
#include "windef.h"

struct Mouse {
	WPARAM btnState;
	POINT curr, prev;
	bool down = false;
	float clickCoolDown = 0.2f, lastPress = 0.f;
};