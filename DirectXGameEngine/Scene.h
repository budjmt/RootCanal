#pragma once

#include "Camera.h"
#include "Shader.h"

class Scene
{
public:
    virtual ~Scene() {};

    virtual void update( float dt ) {};
    virtual void draw() {};

    // For handing mouse input
    virtual void OnMouseDown( WPARAM btnState, int x, int y ) {};
    virtual void OnMouseUp( WPARAM btnState, int x, int y ) {};
    virtual void OnMouseMove( WPARAM btnState, int x, int y ) {};

    Camera** camera() {
        return _camera;
    }

protected:
    Camera** _camera; 
};

enum SceneType {
    MENU,
    GAME,
    DEFAULT
};