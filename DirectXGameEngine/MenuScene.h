#pragma once

#include "Scene.h"
#include "MenuState.h"
#include "StateManager.h"
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

class MenuScene :
    public Scene
{
public:
    MenuScene( Camera** camera, SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader );
    ~MenuScene();

    virtual void update( float dt );

private:
    MenuState* menuState;
    SimplePixelShader* _pixelShader;
    SimpleVertexShader* _vertexShader;
};

