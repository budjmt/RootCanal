#include "GameState.h"

GameState::GameState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
	addGameObject(*_scene->camera());

	DXInfo& dx = DXInfo::getInstance();

	Texture* texture = Texture::createTexture(L"../Assets/texture.png", dx.device, dx.deviceContext);
	Material* material = Material::createMaterial(L"material", texture, vertexShader, pixelShader, _scene->camera());
	Mesh* mesh1 = Mesh::createMesh("../Assets/cone_up.obj");
	Mesh* mesh2 = Mesh::createMesh("../Assets/cube.obj");

	ship = new ColliderObject(mesh1, material);
	ship->rigidBody().floating(true);
	//ship->transform.rotate( vec3( -PI * 0.5f, 0, 0 ) );

	ColliderObject* cube = new ColliderObject(mesh2, material);
	cube->rigidBody().floating(true);
	cube->transform.position(vec3(0, 0, 2));
	cube->transform.scale(vec3(100, 100, 1));

	addGameObject(ship);
	addGameObject(cube);
}

GameState::~GameState()
{
}

void GameState::update(float dt, Mouse* mouse) {
	Keyboard& keys = Keyboard::getInstance();

	if (keys.isDown(VK_LEFT)) {
		ship->transform.rotate(vec3(0, 0, -PI * 2 * dt));
	}
	else if (keys.isDown(VK_RIGHT)) {
		ship->transform.rotate(vec3(0, 0, PI * 2 * dt));
	}

	if (keys.isDown(VK_UP)) {
		RigidBody& rigidBody = ship->rigidBody();
		rigidBody.netForce += dt * 1000 * ship->transform.up();
	}

	vec3 oldCamPos = (*_scene->camera())->transform.position();
	vec3 newCamPos = ship->transform.position();

	DrawDebug::getInstance().drawDebugVector(newCamPos, newCamPos + ship->transform.forward(), vec3(0, 1, 1));
	DrawDebug::getInstance().drawDebugVector(newCamPos, newCamPos + ship->transform.up()	 , vec3(1, 1, 0));
	DrawDebug::getInstance().drawDebugVector(newCamPos, newCamPos + ship->transform.right()	 , vec3(1, 0, 1));

	newCamPos.z = newCamPos.z - 6;

	(*_scene->camera())->transform.position(vec3::lerp(oldCamPos, newCamPos, 0.2f));



	State::update(dt, mouse);
}