#pragma once

#include <DirectXMath.h>
#include "DirectXGameCore.h"

#include "SimpleShader.h"
#include "Mouse.h"

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class Game : public DirectXGameCore {
public:
	Game(HINSTANCE hInstance);
	virtual ~Game();

	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float deltaTime, float totalTime);
	virtual void DrawScene(float deltaTime, float totalTime);

	// For handing mouse input
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

protected:
	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	Mouse mouse;

private:
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* defaultVertexShader;
	SimplePixelShader* defaultPixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix, viewMatrix, projectionMatrix;

	void CreateMatrices();
	void LoadShaders();
};