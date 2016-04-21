#pragma once

#include "Camera.h"
#include "EventDispatcher.h"

class Scene : public EventDispatcher
{
public:
    virtual ~Scene() {};

    virtual void update( float dt ) {};

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
    MENU_SCENE,
    GAME_SCENE,
    DEFAULT
};