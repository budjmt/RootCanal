#pragma once

#include "State.h"

#include "Material.h"
#include "DXInfo.h"
#include "MeshImporter.h"
#include "DrawMesh.h"
#include "ColliderObject.h"
#include "Ship.h"
#include "SimpleShader.h"
#include "Camera.h"

class GameState :
    public State
{
public:
    GameState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader );
    ~GameState();

    virtual void update( float dt, Mouse* mouse );
    virtual void draw( ID3D11DeviceContext* deviceContext );

private:
    Texture* _toonLightingTexture;
	Ship* ship;
    SimplePixelShader* _pixelShader;
    SimpleVertexShader* _vertexShader;

    void updateCamera( float dt );
};

