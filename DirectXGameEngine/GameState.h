#pragma once

#include "State.h"

class GameState :
    public State
{
public:
    GameState( Scene* scene );
    ~GameState();
};

