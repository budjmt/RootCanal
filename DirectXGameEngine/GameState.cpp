#include "GameState.h"

#include "CollisionManager.h"

#include "ToothFront.h"

GameState::GameState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
	addGameObject(*_scene->camera());

    _vertexShader = vertexShader;
    _pixelShader = pixelShader;

	DXInfo& dx = DXInfo::getInstance();

    _toonLightingTexture = Texture::createTexture( L"../Assets/toonlighting.png", dx.device, dx.deviceContext );

	Texture* texture = Texture::createTexture(L"../Assets/texture.png", dx.device, dx.deviceContext);
    Texture* shipTexture = Texture::createTexture( L"../Assets/ship.png", dx.device, dx.deviceContext );
    Material* material = Material::createMaterial(L"material", texture, vertexShader, pixelShader, _scene->camera());
    Material* shipMaterial = Material::createMaterial( L"ship", shipTexture, vertexShader, pixelShader, _scene->camera() );
	Material* material2 = Material::createMaterial(L"material2", texture, vertexShader, Shader::getShader<SimplePixelShader>(L"ToothPixel"), _scene->camera());
    
	Mesh* mesh1 = Mesh::createMesh("../Assets/cone_Z.obj");
	Mesh* mesh2 = Mesh::createMesh("../Assets/cube.obj");

	ship = new Ship(mesh1, shipMaterial);

	ColliderObject* cube = new ColliderObject(mesh2, material);
	cube->rigidBody().floating(true);
	cube->transform.position(vec3(0, 0, 2));
	cube->transform.scale(vec3(100, 100, 1));

	ToothFront* tooth = new ToothFront(mesh2, material2, _scene->camera());
	tooth->transform.position(vec3(0, 0, -1));
	tooth->transform.scale(vec3(100, 100, 1));
	tooth->ship = ship;

	addGameObject(ship);
	addGameObject(cube);
	addGameObject(tooth);
}

GameState::~GameState()
{
}

void GameState::update( float dt, Mouse* mouse ) {

    ship->processMovement( dt );

    updateCamera( dt );

    vec3 shipPos = ship->transform.position();

	DrawDebug::getInstance().drawDebugVector( shipPos, shipPos + ship->transform.forward()              , vec3(0, 1, 1));
	DrawDebug::getInstance().drawDebugVector( shipPos, shipPos + ship->transform.up() + vec3(0,0.001f,0), vec3(1, 1, 0));
	DrawDebug::getInstance().drawDebugVector( shipPos, shipPos + ship->transform.right()                , vec3(1, 0, 1));

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

void GameState::draw( ID3D11DeviceContext* deviceContext )
{
    for( auto g : _gameObjects )
    {
        _pixelShader->SetShaderResourceView( "lightingTexture", _toonLightingTexture->resourceViews[0] );
        g->draw( deviceContext );
    }
}

void GameState::updateCamera( float dt )
{
    vec3 shipPos = ship->transform.position();
    vec3 shipVel = ship->rigidBody().vel();

    vec3 oldCamPos = ( *_scene->camera() )->transform.position();
    vec3 maxCamPos = shipPos;

    // If the ship is moving enough, we'll start moving the camera ahead so the
    // player can see where they're moving
    float seekAheadFactor = minf( 20.f, vec3::length( shipVel ) * .3f );
    maxCamPos += ship->transform.forward() * seekAheadFactor;

    // The camera should pull back from target that it follows (this ship) when the target moves
    maxCamPos.z = shipPos.z - ( 9.5f + seekAheadFactor * .5f );

    vec3 desiredCamPos = vec3::lerp( oldCamPos, maxCamPos, minf( 8.f * dt, 0.65f ) );

    ( *_scene->camera() )->transform.position( desiredCamPos );
}