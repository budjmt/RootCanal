#include "Game.h"

#include <ctime>

using namespace DirectX;

Game::Game(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance) {

}

Game::~Game() {

}

bool Game::Init() {
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if (!DirectXGameCore::Init())
		return false;

	srand((uint32_t)time(NULL));

	CreateMatrices();
	LoadShaders();

	return true;
}

void Game::CreateMatrices() {
	XMMATRIX W = XMMatrixIdentity();
	/*
	XMMATRIX W, T = XMMatrixTranslation(.5f, .75f, .5f), R = XMMatrixRotationY(-PI / 4), S = XMMatrixScaling(1.5f, 1.f, 1.f);
	mat4 w, t = mat4::translate(vec3(0.5f, 0.75f, 0.5f)), r = mat4::rotate(PI / 4, vec3(0,1,0)), s = mat4::scale(vec3(1.5f,1.f,1.f));
	W = XMMatrixMultiply(T, R); W = XMMatrixMultiply(W, S);
	w = t * r * s;
	*/
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH( pos, dir, up);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * 3.1415926535f, aspectRatio, 0.1f,	100.0f);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}

void Game::LoadShaders() {
	defaultVertexShader = new SimpleVertexShader(device, deviceContext);
	defaultVertexShader->LoadShaderFile(L"VertexShader.cso");

	defaultPixelShader = new SimplePixelShader(device, deviceContext);
	defaultPixelShader->LoadShaderFile(L"PixelShader.cso");
}

void Game::UpdateScene(float deltaTime, float totalTime) { }
void Game::DrawScene(float deltaTime, float totalTime) { 
	defaultVertexShader->SetMatrix4x4("world", worldMatrix);
	defaultVertexShader->SetMatrix4x4("inv_trans_world", worldMatrix);
	defaultVertexShader->SetMatrix4x4("view", viewMatrix);
	defaultVertexShader->SetMatrix4x4("projection", projectionMatrix);

	defaultVertexShader->SetShader(true);
	defaultPixelShader->SetShader(true);
}

void Game::OnResize() { 
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,		  	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

void Game::OnMouseDown(WPARAM btnState, int x, int y) { 
	// Save the previous mouse position, so we have it for the future
	mouse.btnState = btnState;
	//mouse.prev.x = mouse.curr.x;
	//mouse.prev.y = mouse.curr.y;
	mouse.curr.x = x;
	mouse.curr.y = y;
	mouse.lastPress = GetTickCount64();
	mouse.down = true;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);

    currScene->OnMouseDown( btnState, x, y );
}

void Game::OnMouseUp(WPARAM btnState, int x, int y) { 
	mouse.btnState = btnState;
	mouse.down = false;

    currScene->OnMouseUp( btnState, x, y );

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

void Game::OnMouseMove(WPARAM btnState, int x, int y) { 
	// Save the previous mouse position, so we have it for the future
	mouse.btnState = btnState;
	//mouse.prev.x = mouse.curr.x;
	//mouse.prev.y = mouse.curr.y;
	mouse.curr.x = x;
	mouse.curr.y = y;

    currScene->OnMouseMove( btnState, x, y );
}