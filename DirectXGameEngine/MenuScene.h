#pragma once

#include "Scene.h"
#include "MenuState.h"
#include "StateManager.h"
#include "Light.h"

class MenuScene :
    public Scene
{
public:
    MenuScene( Camera** camera );
    ~MenuScene();

    virtual void update( float dt );
    virtual void draw();

private:
    MenuState* menuState;
    SimplePixelShader* _pixelShader;
    SimpleVertexShader* _vertexShader;

    DirectionalLight _l1, _l2;
};

