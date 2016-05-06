#include "MenuState.h"

MenuState::MenuState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
	addGameObject(*_scene->camera());

	DXInfo& dx = DXInfo::getInstance();

	Texture* texture = Texture::createTexture(L"../Assets/menu.png", dx.device, dx.deviceContext);
	Material* material = Material::createMaterial(L"dumbmaterial", texture, vertexShader, pixelShader, _scene->camera());
	Mesh* mesh = Mesh::createMesh("../Assets/cube.obj");

	ColliderObject* obj1 = new ColliderObject(mesh, material);
	obj1->rigidBody().floating(true);
	obj1->transform.rotate(vec3(0, 0, PI * 0.5f));
	obj1->transform.scale(vec3(6, 6, 1));

	addGameObject(obj1);
}

MenuState::~MenuState()
{
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
        State::update( dt, mouse );
    }
}