#include "MenuState.h"

MenuState::MenuState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
	addGameObject(*_scene->camera());

	DXInfo& dx = DXInfo::getInstance();

	text = new UIText();
	addGameObject(text);
}

void MenuState::update(float dt, Mouse* mouse) {
	Keyboard& keys = Keyboard::getInstance();

	if (keys.isDown(' '))
	{
		CollisionManager::getInstance().clear();
		StateManager::getInstance().changeScene( SceneType::GAME );
	}
    else
    {
		text->drawText(L"PRESS SPACE", vec3(300, 300, 0), CENTER, vec4(.9f,.9f,.9f,1.f));
		text->drawText(L"TO PLAY", vec3(300, 400, 0), CENTER, vec4(.9f, .9f, .9f, 1.f));

        text->drawText( L"ARROWS : MOVE", vec3( 900, 300, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );
        text->drawText( L"TAB : XRAY", vec3( 900, 400, 0 ), CENTER, vec4( .9f, .9f, .9f, 1.f ) );

        State::update( dt, mouse );
    }
}