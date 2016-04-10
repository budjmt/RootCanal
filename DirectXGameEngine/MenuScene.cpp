#include "MenuScene.h"

MenuScene::MenuScene( Camera** camera, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
{
	_camera = camera;

    menuState = new MenuState( this, vertexShader, pixelShader );
    StateManager::getInstance().gameState( menuState );

    _pixelShader = pixelShader;
    _vertexShader = vertexShader;
}

MenuScene::~MenuScene()
{
    delete menuState;
}

void MenuScene::update( float dt )
{
}
