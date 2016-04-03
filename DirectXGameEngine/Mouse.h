#pragma once
#include "windef.h"

struct Mouse {
    WPARAM btnState;
    POINT curr, prev;
    bool down = false;
    ULONGLONG clickCoolDown = 200, lastPress = 0;
};