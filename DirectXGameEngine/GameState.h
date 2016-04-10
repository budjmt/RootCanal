#pragma once

#include "State.h"

#include "Material.h"
#include "DXInfo.h"
#include "MeshImporter.h"
#include "DrawMesh.h"
#include "ColliderObject.h"
#include "SimpleShader.h"
#include "Camera.h"

class GameState :
    public State
{
public:
    GameState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader );
    ~GameState();

	void update(float dt, Mouse* mouse);

private:
	ColliderObject* ship;
};

