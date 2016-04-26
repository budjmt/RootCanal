#include "GameScene.h"

using namespace DirectX;

GameScene::GameScene( Camera** camera )
{
	_camera = camera;

    DXInfo& dx = DXInfo::getInstance();

    _vertexShader = Shader::getShader<SimpleVertexShader>( L"BasicVertex" );
    _pixelShader = Shader::getShader<SimplePixelShader>( L"ToonPixel" );

    _l1 = {
        XMFLOAT4( 0.7f, 0.65f, 0.8f, .9f ),
        XMFLOAT4( 0.8f, 1.f, 1.f, .9f ),
        XMFLOAT3( 0.1f, -1.f, 0.f )
    };

    _l2 = {
        XMFLOAT4( 0.05f, 0.1f, 0.05f, .5f ),
        XMFLOAT4( 0.6f, 0.f, 0.25f, .1f ),
        XMFLOAT3( -1.f, 1.f, -0.5f )
    };

	gameState = new GameState( this, _vertexShader, _pixelShader );
    StateManager::getInstance().gameState( gameState );
}


GameScene::~GameScene()
{
    delete gameState;
}

void GameScene::update(float dt)
{
}

void GameScene::draw()
{
    auto& dx = DXInfo::getInstance();

    ( *_camera )->updateCamMat( _vertexShader );

    _pixelShader->SetData( "light1", &_l1, sizeof( DirectionalLight ) );
    _pixelShader->SetData( "light2", &_l2, sizeof( DirectionalLight ) );

    // Optimize for transparent/non-transparent stuff later
    dx.deviceContext->OMSetBlendState( dx.blendState, NULL, 0xffff );
    StateManager::getInstance().draw( dx.deviceContext );
}
