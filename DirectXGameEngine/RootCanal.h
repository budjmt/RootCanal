#pragma once

#include "Game.h"

#include "DXInfo.h"
#include "GameObject.h"
#include "Camera.h"
#include "DrawMesh.h"
#include "Light.h"
#include "CollisionManager.h"
#include "StateManager.h"
#include "SceneEvent.h"
#include "Shader.h"

#include "MenuScene.h"
#include "GameScene.h"
#include "PostProcess.h"
#include "PostChainManager.h"

class RootCanal : public Game
{
public:
	RootCanal(HINSTANCE hInstance);
	~RootCanal();

	// Overrides for base level methods
	bool Init() override;
	void OnResize() override;
	void UpdateScene(float deltaTime, float totalTime) override;
	void DrawScene(float deltaTime, float totalTime) override;

    void SetScene( Scene* scene );

private:
	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void LoadShaders();
	void CreateGeometry();
	void CreateMatrices();

	void setupRenderTarget();

	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11RenderTargetView* ppRTV;
	ID3D11ShaderResourceView* ppSRV;

	PostProcess* post;
	PostChainManager* postManager;

	Camera* camera = nullptr;
};