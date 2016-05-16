#include "GameState.h"

#include "CollisionManager.h"

#include "Ship.h"

#include "ToothFront.h"

GameState::GameState( Scene* scene, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader )
    : State( scene )
{
    addGameObject( *_scene->camera() );

    _vertexShader = vertexShader;
    _pixelShader = pixelShader;

    DXInfo& dx = DXInfo::getInstance();

    _toonLightingTexture = Texture::createTexture( L"../Assets/toonlighting.png", dx.device, dx.deviceContext );


    Texture* whiteTexture = Texture::createTexture( L"../Assets/white.png", dx.device, dx.deviceContext );
    whiteTexture->addTex( _toonLightingTexture->resourceViews[0] );

    Texture* toothTexture = Texture::createTexture( L"../Assets/toothCloud.png", dx.device, dx.deviceContext );
    toothTexture->addTex( _toonLightingTexture->resourceViews[0] );

    Texture* texture = Texture::createTexture( L"../Assets/texture.png", dx.device, dx.deviceContext );
    texture->addTex( _toonLightingTexture->resourceViews[0] );
    Texture* shipTexture = Texture::createTexture( L"../Assets/shipDiffuse.png", dx.device, dx.deviceContext );
    shipTexture->addTex( _toonLightingTexture->resourceViews[0] );

    Material* material = Material::createMaterial( L"material", texture, vertexShader, pixelShader, _scene->camera() );
    Material* shipMaterial = Material::createMaterial( L"ship", shipTexture, vertexShader, pixelShader, _scene->camera() );

    Material* material2 = Material::createMaterial( L"material2", toothTexture, vertexShader, Shader::getShader<SimplePixelShader>( L"ToothPixel" ), _scene->camera() );
    Material* material3 = Material::createMaterial( L"material3", whiteTexture, vertexShader, pixelShader, _scene->camera() );


    Mesh* mesh1 = Mesh::createMesh( "../Assets/ship.obj" );
    Mesh* mesh2 = Mesh::createMesh( "../Assets/cube.obj" );

    ship = new Ship( mesh1, shipMaterial );
    ship->transform.rotate( 3.1415f * -.5f, vec3( 0, 0, -1 ) );
    ship->transform.position( vec3( -55, 0, 0 ) );
    CollisionManager::getInstance().addObject( ship );

    vec3 camPos = ship->transform.position();
    camPos.z -= 5.f;

    ( *_scene->camera() )->transform.position( camPos );

    cannon = new Cannon( mesh2, material, mesh2, material, this, ship );
    cannon->transform.position( vec3( 0, 5, 0 ) );
    cannon->transform.scale( vec3( 2, 2, 1 ) );
    cannon->spawnBullets( 8 );
    CollisionManager::getInstance().addObject( cannon );

    ColliderObject* cube = new ColliderObject( mesh2, material3 );
    cube->rigidBody().floating( true );
    cube->transform.position( vec3( 0, 0, 2 ) );
    cube->transform.scale( vec3( 100, 100, 1 ) );
    cube->rigidBody().solid( 0 );

    ToothFront* tooth = new ToothFront( mesh2, material2, _scene->camera(), dx.device );
    tooth->transform.position( vec3( 0, 0, -1 ) );
    tooth->transform.scale( vec3( 100, 100, 1 ) );
    tooth->ship = ship;

    text = new UIText();

    addGameObject( ship );
    addGameObject( cannon );
    addGameObject( cube );
    addGameObject( tooth );
    addGameObject( text );

    score = 0;

    //audioManager = new AudioManager();
}

#include "Keyboard.h"
void GameState::toggleRenderMode( bool force ) {
	Keyboard& keys = Keyboard::getInstance();

	static bool keyDown = false;
	if (force || (keys.isDown(VK_TAB) && !keyDown)) {
		keyDown = true;
		if (renderSwap % 2 == 0) {
			for (auto m : Material::loadedMaterials) {
				m.second->setOpacity(0.6f);
			}

             ship->usingXray( true );
		}
		else {
			for (auto m : Material::loadedMaterials) {
				m.second->setOpacity(1);
			}

            ship->usingXray( false );
		}
		renderSwap++;
	}
    if (!keys.isDown(VK_TAB) && keyDown) {
		keyDown = false;
	}
}

void GameState::update( float dt, Mouse* mouse ) {
	toggleRenderMode();

	ship->update( dt );
	cannon->update( dt );

    updateCamera( dt );

    if( ship->usingXray() && ship->getXray() == 0 )
    {
        toggleRenderMode( true );
        StateManager::getInstance().forceXraySwitch( true );
    }

    vec3 shipPos = ship->transform.position();

    // If the ship is where the tooth is, increase the score!
    if( shipPos.x >= -50 && shipPos.x <= 50 && shipPos.y >= -50 && shipPos.y <= 50 )
    {
        score++;
    }

	DrawDebug::getInstance().drawDebugVector( shipPos, shipPos + ship->transform.forward()              , vec3(0, 1, 1));
	DrawDebug::getInstance().drawDebugVector( shipPos, shipPos + ship->transform.up() + vec3(0,0.001f,0), vec3(1, 1, 0));
	DrawDebug::getInstance().drawDebugVector( shipPos, shipPos + ship->transform.right()                , vec3(1, 0, 1));

    vec4 color;
    if( renderSwap % 2 != 0 )
    {
        color = vec4( 0, 0, 0, 1 );
    }
    else
    {
        color = vec4( 1, 1, 1, 1 );
    }

    text->drawText( L"HP:", vec3(0, 0, 0), TextJustify::LEFT, color );
    text->drawText( std::to_wstring((int)ship->getHealth()), vec3( 150, 0, 0 ), TextJustify::LEFT, color );

    text->drawText( L"XRAY:", vec3( 0, 50, 0 ), TextJustify::LEFT, color );
    text->drawText( std::to_wstring( (int)ship->getXray() ), vec3( 150, 50, 0 ), TextJustify::LEFT, color );

    text->drawText( L"SCORE:", vec3( 0, 100, 0 ), TextJustify::LEFT, color );
    text->drawText( std::to_wstring( score ), vec3( 150, 100, 0 ), TextJustify::LEFT, color );

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

    if( ship->getHealth() <= 0 )
    {
        CollisionManager::getInstance().clear();
        StateManager::getInstance().changeScene( SceneType::GAME_OVER );
    }
}

void GameState::draw( ID3D11DeviceContext* deviceContext )
{
    for( auto g : _gameObjects )
    {
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
    float seekAheadFactor = minf( 40.f, vec3::length( shipVel ) * .4f );
    maxCamPos += ship->transform.forward() * seekAheadFactor;

    // The camera should pull back from target that it follows (this ship) when the target moves
    maxCamPos.z = shipPos.z - ( 9.5f + seekAheadFactor );

    vec3 desiredCamPos = vec3::lerp( oldCamPos, maxCamPos, minf( 8.f * dt, 0.65f ) );

    ( *_scene->camera() )->transform.position( desiredCamPos );
}