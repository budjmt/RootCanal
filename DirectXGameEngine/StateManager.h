#pragma once

#include "State.h"
#include "Mouse.h"

class StateManager
{
public:
    // Singleton pattern
    static StateManager& getInstance()
    {
        static StateManager instance;
        return instance;
    }

    State* gameState() {
        return _state;
    }
    void gameState( State* s ) {
        _state = s;
    }

    void update( float dt, Mouse* mouse ) {
        _state->update( dt, mouse );
    }

    void draw( ID3D11DeviceContext* deviceContext ) {
        _state->draw( deviceContext );
    }

    bool nextScene() {
         return _nextScene;
    }
    void nextScene( bool s )
    {
        _nextScene = s;
    }

private:
    StateManager() {}
    StateManager( StateManager const& ) = delete;
    void operator=( StateManager const& ) = delete;

    State* _state;
    bool _nextScene;
};