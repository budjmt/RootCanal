#pragma once

#include "State.h"
#include "Scene.h"

class MenuState :
    public State
{
public:
    MenuState( Scene* scene );
    ~MenuState();
};

