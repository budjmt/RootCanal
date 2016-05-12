#include "GameOverState.h"

GameOverState::GameOverState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
    addGameObject( *_scene->camera() );

    DXInfo& dx = DXInfo::getInstance();

    text = new UIText();
    addGameObject( text );
}

void GameOverState::update( float dt, Mouse* mouse ) {
    Keyboard& keys = Keyboard::getInstance();

    if( keys.isDown( ' ' ) )
    {
        CollisionManager::getInstance().clear();
        StateManager::getInstance().changeScene( SceneType::GAME );
    }
    else
    {
        text->drawText( L"GAME OVER", vec3( 300, 300, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );
        text->drawText( L"PRESS SPACE", vec3( 300, 400, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );
        text->drawText( L"TO TRY AGAIN", vec3( 300, 500, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );
        State::update( dt, mouse );
    }
}