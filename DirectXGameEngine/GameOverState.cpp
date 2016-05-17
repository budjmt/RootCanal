#include "GameOverState.h"

GameOverState::GameOverState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
    addGameObject( *_scene->camera() );

    DXInfo& dx = DXInfo::getInstance();

    text = new UIText();
    addGameObject( text );

	audioManager = new AudioManager();
	audioManager->playFile(_TEXT("../Assets/Death.wav"), 0.7f, false);
	audioManager->playFile(_TEXT("../Assets/GameOverMusic.wav"), 1.0f, true);
}

void GameOverState::update( float dt, Mouse* mouse ) {
    Keyboard& keys = Keyboard::getInstance();

    if( keys.isDown( ' ' ) )
    {
		audioManager->stopSound();
        CollisionManager::getInstance().clear();
        StateManager::getInstance().changeScene( SceneType::GAME );
    }
    else
    {
        text->drawText( L"GAME OVER", vec3( 600, 250, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );
        text->drawText( L"PRESS SPACE", vec3( 600, 350, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );
        text->drawText( L"TO TRY AGAIN", vec3( 600, 450, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );
        State::update( dt, mouse );
    }
}