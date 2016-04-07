#include "SceneTransitionState.h"

SceneTransitionState::SceneTransitionState( Scene* oldScene, Scene* newScene )
    : State( oldScene )
{
    _oldScene = oldScene;
    _newScene = newScene;
    _state = StateManager::getInstance().gameState();
}

SceneTransitionState::~SceneTransitionState()
{
}

Scene* SceneTransitionState::getOldScene()
{
    return _oldScene;
}

Scene* SceneTransitionState::getNewScene()
{
    return _newScene;
}

State* SceneTransitionState::getState()
{
    return _state;
}
