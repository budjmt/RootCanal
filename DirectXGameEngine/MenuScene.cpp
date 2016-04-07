#include "MenuScene.h"

MenuScene::MenuScene( Camera** camera, SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader )
{
    menuState = new MenuState( this );
    StateManager::getInstance().gameState( menuState );

    _pixelShader = pixelShader;
    _vertexShader = vertexShader;

    _camera = camera;
    menuState->addGameObject( *camera );

    DXInfo& dx = DXInfo::getInstance();

    Texture* texture = Texture::createTexture( L"../Assets/menu.png", dx.device, dx.deviceContext );
    Material* material = Material::createMaterial( L"material", texture, vertexShader, pixelShader, camera );
    Mesh* mesh = Mesh::createMesh( "../Assets/cube.obj" );

    ColliderObject* obj1 = new ColliderObject( mesh, material );
    obj1->rigidBody().floating( true );
    obj1->transform.rotate( vec3(0, 0, PI * 0.5f) );
    obj1->transform.scale( vec3( 6, 6, 1 ) );

    menuState->addGameObject( obj1 );
}

MenuScene::~MenuScene()
{
    delete menuState;
}

void MenuScene::update( float dt )
{
    Keyboard& keys = Keyboard::getInstance();

    if( keys.isDown( ' ' ) )
    {
        CollisionManager::getInstance().clear();

        StateManager::getInstance().nextScene( true );
       /* SceneTransitionState* transition = new SceneTransitionState( this, gameScene );
        StateManager::getInstance().gameState( transition );
        dispatchEvent( SceneEvent::CHANGE );*/
    }
}
