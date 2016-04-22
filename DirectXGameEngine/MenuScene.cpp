#include "MenuScene.h"

using namespace DirectX;

MenuScene::MenuScene( Camera** camera )
{
	_camera = camera;

    _vertexShader = Shader::getShader<SimpleVertexShader>( L"BasicVertex" );
    _pixelShader = Shader::getShader<SimplePixelShader>( L"BasicPixel" );

    _l1 = {
        XMFLOAT4( 0.f,0.1f,0.1f,1.f ),
        XMFLOAT4( 0.8f,1.f,0,1.f ),
        XMFLOAT3( 0.5f,-1.f,1.f )
    };

    _l2 = {
        XMFLOAT4( 0.05f,0.1f,0.05f,1.f ),
        XMFLOAT4( 0.6f,0.f,0.25f,1.f ),
        XMFLOAT3( -1.f,1.f,-0.5f )
    };

    menuState = new MenuState( this, _vertexShader, _pixelShader );
    StateManager::getInstance().gameState( menuState );
}

MenuScene::~MenuScene()
{
    delete menuState;
}

void MenuScene::update( float dt )
{
}

void MenuScene::draw()
{
    auto& dx = DXInfo::getInstance();

    ( *_camera )->updateCamMat( _vertexShader );

    _pixelShader->SetData( "light1", &_l1, sizeof( DirectionalLight ) );
    _pixelShader->SetData( "light2", &_l2, sizeof( DirectionalLight ) );

    // Optimize for transparent/non-transparent stuff later
    dx.deviceContext->OMSetBlendState( dx.blendState, NULL, 0xffff );
    StateManager::getInstance().draw( dx.deviceContext );
}
