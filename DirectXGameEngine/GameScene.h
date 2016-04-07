#pragma once

#include "Scene.h"
#include "GameState.h"
#include "StateManager.h"
#include "Material.h"
#include "DXInfo.h"
#include "MeshImporter.h"
#include "DrawMesh.h"
#include "ColliderObject.h"
#include "SimpleShader.h"
#include "Camera.h"

class GameScene :
    public Scene
{
public:
    GameScene( Camera** camera, SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader );
    ~GameScene();

    virtual void update( float dt );

private:
    GameState* gameState;
    ColliderObject* ship;
};

