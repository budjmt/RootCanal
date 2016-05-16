#pragma once

#include "State.h"
#include "Scene.h"
#include "Material.h"
#include "DXInfo.h"
#include "MeshImporter.h"
#include "DrawMesh.h"
#include "ColliderObject.h"
#include "SimpleShader.h"
#include "Keyboard.h"
#include "Camera.h"
#include "GameScene.h"
#include "SceneTransitionState.h"
#include "SceneEvent.h"
#include "CollisionManager.h"

class GameOverState :
    public State
{
public:
    GameOverState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader );

    void update( float dt, Mouse* mouse );

private:
    UIText* text;
	AudioManager* audioManager;
};

