#pragma once

#include "State.h"
#include "Scene.h"
#include "StateManager.h"

class SceneTransitionState :
    public State
{
public:
    SceneTransitionState( Scene* oldScene, Scene* newScene );
    ~SceneTransitionState();

    Scene* getOldScene();
    Scene* getNewScene();
    State* getState();

private:
    Scene* _oldScene;
    Scene* _newScene;
    State* _state;
};

