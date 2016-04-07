#pragma once

#include "Game.h"

#include "DXInfo.h"
#include "GameObject.h"
#include "Player.h"
#include "Camera.h"
#include "DrawMesh.h"
#include "Light.h"
#include "CollisionManager.h"

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

	// For handing mouse input
	//void OnMouseDown(WPARAM btnState, int x, int y);
	//void OnMouseUp(WPARAM btnState, int x, int y);
	//void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void LoadShaders();
	void CreateGeometry();
	void CreateMatrices();

	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	Camera* camera = nullptr;
	std::vector<GameObject*> gameObjects;
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;

	Player* player;

	DirectionalLight l1, l2;
};