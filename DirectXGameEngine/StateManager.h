#pragma once

#include "State.h"
#include "Scene.h"
#include "Mouse.h"
#include <DirectXMath.h>

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

    SceneType changeScene() {
         return _nextSceneType;
    }
    void changeScene( SceneType s )
    {
        _nextSceneType = s;
    }

    void xrayMode( bool value ) {
        _xrayMode = value;
    }
    bool xrayMode() {
        return _xrayMode;
    }

private:
    StateManager() {}
    StateManager( StateManager const& ) = delete;
    void operator=( StateManager const& ) = delete;

    State* _state;
    SceneType _nextSceneType = SceneType::DEFAULT;
    bool _xrayMode = false;
};