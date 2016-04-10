#pragma once

#include "Scene.h"
#include "GameState.h"
#include "StateManager.h"

class GameScene :
    public Scene
{
public:
    GameScene( Camera** camera, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader );
    ~GameScene();

    virtual void update( float dt );

private:
    GameState* gameState;
};

