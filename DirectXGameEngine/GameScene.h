#pragma once

#include "Scene.h"
#include "GameState.h"
#include "StateManager.h"
#include "Light.h"

class GameScene :
    public Scene
{
public:
    GameScene( Camera** camera );
    ~GameScene();

    virtual void update( float dt );
    virtual void draw();

private:
    GameState* gameState;
    SimplePixelShader* _pixelShader;
    SimpleVertexShader* _vertexShader;

    DirectionalLight _l1, _l2;
};

