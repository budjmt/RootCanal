#pragma once

#include "Scene.h"
#include "MenuState.h"
#include "StateManager.h"

class MenuScene :
    public Scene
{
public:
    MenuScene( Camera** camera, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader );
    ~MenuScene();

    virtual void update( float dt );

private:
    MenuState* menuState;
    SimplePixelShader* _pixelShader;
    SimpleVertexShader* _vertexShader;
};

