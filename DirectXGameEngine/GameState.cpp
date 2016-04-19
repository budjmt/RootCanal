#include "GameState.h"

#include "CollisionManager.h"

GameState::GameState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
	addGameObject(*_scene->camera());

	DXInfo& dx = DXInfo::getInstance();

	Texture* texture = Texture::createTexture(L"../Assets/texture.png", dx.device, dx.deviceContext);
	Material* material = Material::createMaterial(L"material", texture, vertexShader, pixelShader, _scene->camera());
	Mesh* mesh1 = Mesh::createMesh("../Assets/cone_Z.obj");
	Mesh* mesh2 = Mesh::createMesh("../Assets/cube.obj");

	ship = new Ship(mesh1, material);

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

	ship->processMovement(dt);

	vec3 oldCamPos = (*_scene->camera())->transform.position();
	vec3 newCamPos = ship->transform.position();

	DrawDebug::getInstance().drawDebugVector(newCamPos, newCamPos + ship->transform.forward(), vec3(0, 1, 1));
	DrawDebug::getInstance().drawDebugVector(newCamPos, newCamPos + ship->transform.up() + vec3(0,0.001f,0), vec3(1, 1, 0));
	DrawDebug::getInstance().drawDebugVector(newCamPos, newCamPos + ship->transform.right()	 , vec3(1, 0, 1));

	newCamPos.z = newCamPos.z - 6;

	(*_scene->camera())->transform.position(vec3::lerp(oldCamPos, newCamPos, minf(10.f * dt, 1.f)));

	State::update(dt, mouse);
	CollisionManager::getInstance().update(dt);

#if DEBUG
	DrawDebug& d = DrawDebug::getInstance();
	//d.drawDebugSphere(vec3(0.5f, 0.5f, 0.5f), 0.5f);
	d.drawDebugVector(vec3(), vec3(1, 0, 0), vec3(1, 0, 0));
	d.drawDebugVector(vec3(), vec3(0, 1, 0), vec3(0, 0, 1));
	d.drawDebugVector(vec3(), vec3(0, .001f, 1), vec3(0, 1, 0));
	//float c = 2 * PI;
	//vec3 v(1, 0, 0);
	//int div = 12;
	//for (int i = 0; i < div; i++) for (int j = 0; j < div; j++) d.drawDebugVector(vec3(), (vec3)(mat4::rotate(c * i / div, vec3(1, 0, 0)) * mat4::rotate(c * j / div, vec3(0, 1, 0)) * vec4(v)));
	CollisionManager::getInstance().draw();
#endif
}