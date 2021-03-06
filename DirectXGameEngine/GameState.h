#pragma once

#include "State.h"

#include "Material.h"
#include "DXInfo.h"
#include "MeshImporter.h"
#include "DrawMesh.h"
#include "ColliderObject.h"
#include "StateManager.h"

#include "UIText.h"
#include "Cannon.h"
#include "SimpleShader.h"
#include "Camera.h"
#include "AudioManager.h"
#include <tchar.h>

class Ship;

class GameState :
    public State
{
public:
    GameState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader );

    virtual void update( float dt, Mouse* mouse );
    virtual void draw( ID3D11DeviceContext* deviceContext );
	void toggleRenderMode( bool force = false );
private:
	UIText* text;
    Texture* _toonLightingTexture;
	Ship* ship;
    std::vector<Cannon*> cannons;
    SimplePixelShader* _pixelShader;
    SimpleVertexShader* _vertexShader;
	AudioManager* audioManager;

    void updateCamera( float dt );
	int renderSwap = 0;
    int score;
};

