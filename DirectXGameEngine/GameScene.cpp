#include "GameScene.h"

GameScene::GameScene( Camera** camera, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
{
	_camera = camera;

	gameState = new GameState( this, vertexShader, pixelShader );
    StateManager::getInstance().gameState( gameState );
}


GameScene::~GameScene()
{
    delete gameState;
}

void GameScene::update(float dt)
{
}