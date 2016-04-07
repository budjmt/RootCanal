#include "GameScene.h"

GameScene::GameScene( Camera** camera, SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader )
{
    gameState = new GameState( this );
    StateManager::getInstance().gameState( gameState );

    _camera = camera;
    gameState->addGameObject( *camera );

    DXInfo& dx = DXInfo::getInstance();

    Texture* texture = Texture::createTexture( L"../Assets/texture.png", dx.device, dx.deviceContext );
    Material* material = Material::createMaterial( L"material", texture, vertexShader, pixelShader, camera );
    Mesh* mesh1 = Mesh::createMesh( "../Assets/cone_offset.obj" );
    Mesh* mesh2 = Mesh::createMesh( "../Assets/cube.obj" );

    ship = new ColliderObject( mesh1, material );
    ship->transform.rotate( vec3( PI, 0, 0 ) );

    ColliderObject* cube = new ColliderObject( mesh2, material );
    cube->transform.position( vec3( 0, 0, 20 ) );
    cube->transform.scale( vec3( 100, 100, 1 ) );

    gameState->addGameObject( ship );
    gameState->addGameObject( cube );
}


GameScene::~GameScene()
{
    delete gameState;
}

void GameScene::update( float dt )
{
    Keyboard& keys = Keyboard::getInstance();

    if( keys.isDown( VK_LEFT ) ) {
        ship->transform.rotate( vec3( 0, 0, -0.003f ) );
    }
    else if( keys.isDown( VK_RIGHT ) ) {
        ship->transform.rotate( vec3( 0, 0, 0.003f ) );
    }

    vec3 newPos = ship->transform.position();
    newPos += vec3::cross(ship->transform.forward(), ship->transform.right()) * 0.06f;
    ship->transform.position( newPos );

    vec3 oldCamPos = ( *_camera )->transform.position();
    vec3 newCamPos = oldCamPos;
    newCamPos.x = newPos.x;
    newCamPos.y = newPos.y;
    newCamPos.z = newPos.z - 6;

    vec3 displacement = newCamPos - oldCamPos;

    ( *_camera )->transform.position( oldCamPos + displacement * 0.3f );
}