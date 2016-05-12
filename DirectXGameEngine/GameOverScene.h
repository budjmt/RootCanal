#pragma once

#include "Scene.h"
#include "GameOverState.h"
#include "StateManager.h"
#include "Light.h"

class GameOverScene :
    public Scene
{
public:
    GameOverScene( Camera** camera );
    ~GameOverScene();

    virtual void update( float dt );
    virtual void draw();

private:
    GameOverState* gameOverState;
    SimplePixelShader* _pixelShader;
    SimpleVertexShader* _vertexShader;

    DirectionalLight _l1, _l2;
};

