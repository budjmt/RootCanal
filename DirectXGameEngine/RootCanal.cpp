// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "RootCanal.h"

#include "MeshImporter.h"
#include "DrawDebug.h"

// For the DirectX Math library
using namespace DirectX;

//this is done on purpose; the hpp file contains a Win32 entry point function
#define GAME_CLASS RootCanal
#include "GameEntryPoint.hpp"

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
RootCanal::RootCanal(HINSTANCE hInstance)
	: Game(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	wchar_t cwd[2000];
	_wgetcwd(cwd, 2000);
	//windowCaption = cwd;
	windowCaption = L"Root Canal Prototype";

	// Custom window size - will be created by Init() later
	windowWidth = 1280;
	windowHeight = 720;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
RootCanal::~RootCanal()
{
	delete vertexShader;
    delete pixelShader;

	for (auto m : Mesh::loadedMeshes )
		delete m.second;
	for (auto m : Material::loadedMaterials )
		delete m.second;
    for( auto pair : Texture::loadedTextures )
        delete pair.second;

    delete currScene;
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool RootCanal::Init()
{
	if (!Game::Init())
		return false;

	DXInfo& d = DXInfo::getInstance();
	d.device = device;
	d.deviceContext = deviceContext;

	d.rasterDesc.CullMode = D3D11_CULL_BACK;
	//d.rasterDesc.CullMode = D3D11_CULL_NONE;
	d.rasterDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&d.rasterDesc, &d.rasterState);
	deviceContext->RSSetState(d.rasterState);

	d.blendDesc.IndependentBlendEnable = false;
	d.blendDesc.AlphaToCoverageEnable = false;
	d.blendDesc.RenderTarget[0].BlendEnable = true;
	d.blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	d.blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	d.blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	d.blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	d.blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	d.blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	d.blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	d.device->CreateBlendState(&d.blendDesc, &d.blendState);

	d.swapChain = swapChain;
	d.depthStencilBuffer = depthStencilBuffer;
	d.renderTargetView = renderTargetView;
	d.depthStencilView = depthStencilView;
	d.viewport = &viewport;
	d.driverType = &driverType;
	d.featureLevel = &featureLevel;

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	LoadShaders();
	CreateGeometry();
	CreateMatrices();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    menuScene = new MenuScene( &camera, vertexShader, pixelShader );

    SetScene( menuScene );

	// Successfully initialized
	return true;
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void RootCanal::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	assert(vertexShader->LoadShaderFile(L"VertexShader.cso"));

	pixelShader = new SimplePixelShader(device, deviceContext);
	assert(pixelShader->LoadShaderFile(L"PixelShader.cso"));
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void RootCanal::CreateGeometry()
{

#if DEBUG
	DrawDebug::getInstance().camera(&camera);
#endif

	l1 = {
		XMFLOAT4(0.f,0.1f,0.1f,1.f),
		XMFLOAT4(0.8f,1.f,0,1.f),
		XMFLOAT3(0.5f,-1.f,1.f)
	};

	l2 = {
		XMFLOAT4(0.05f,0.1f,0.05f,1.f),
		XMFLOAT4(0.6f,0.f,0.25f,1.f),
		XMFLOAT3(-1.f,1.f,-0.5f)
	};
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void RootCanal::CreateMatrices()
{
    // TODO: Clean this up and everything involving cameras and transforms
    camera = new Camera();
    OnResize();
	camera->transform.position( vec3(0.f, 0.f, -5.f) );
	//camera->transform.rotate(0, PI, 0);
	camera->update(0.f, &mouse);
	camera->updateProjection(windowWidth, windowHeight, aspectRatio);
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void RootCanal::OnResize()
{
	Game::OnResize();
	if (camera) camera->updateProjection(windowWidth, windowHeight, aspectRatio);
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
void RootCanal::UpdateScene(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

    currScene->update( deltaTime );

    if( StateManager::getInstance().nextScene() )
    {
        CreateMatrices();
        gameScene = new GameScene( &camera, vertexShader, pixelShader );
        SetScene( gameScene );
        StateManager::getInstance().nextScene( false );
        OnResize();
    }
    else
    {
        StateManager::getInstance().update( deltaTime, &mouse );
        CollisionManager::getInstance().update( deltaTime );
    }

    mouse.prev.x = mouse.curr.x;
    mouse.prev.y = mouse.curr.y;
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void RootCanal::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	camera->updateCamMat(vertexShader);

	pixelShader->SetData("light1", &l1, sizeof(DirectionalLight));
	pixelShader->SetData("light2", &l2, sizeof(DirectionalLight));

	//optimize for transparent/non-transparent stuff later
	auto& dx = DXInfo::getInstance();
	dx.deviceContext->OMSetBlendState(dx.blendState, NULL, 0xffff);
    StateManager::getInstance().draw( deviceContext );
#if DEBUG
	DrawDebug& d = DrawDebug::getInstance();
	//d.drawDebugSphere(vec3(0.5f, 0.5f, 0.5f), 0.5f);
	d.drawDebugVector(vec3(), vec3(1, 0, 0), vec3(1, 0, 0));
	d.drawDebugVector(vec3(), vec3(0, 1, 0), vec3(0, 0, 1));
	d.drawDebugVector(vec3(), vec3(0, .001f, 1), vec3(0, 1, 0));
	float c = 2 * PI;
	//vec3 v(1, 0, 0);
	//int div = 12;
	//for (int i = 0; i < div; i++) for (int j = 0; j < div; j++) d.drawDebugVector(vec3(), (vec3)(mat4::rotate(c * i / div, vec3(1, 0, 0)) * mat4::rotate(c * j / div, vec3(0, 1, 0)) * vec4(v)));
	d.draw();

	CollisionManager::getInstance().draw();
#endif

	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}

void RootCanal::OnSceneChange( Event e )
{
    SceneTransitionState* transition = static_cast<SceneTransitionState*>( StateManager::getInstance().gameState() );
    SetScene( transition->getNewScene() );

    camera = *(currScene->camera());
    StateManager::getInstance().gameState( transition->getState() );

    RootCanal::OnResize();
}

void RootCanal::SetScene( Scene* scene )
{
    if( currScene ) delete currScene;

    currScene = scene;

    // Add event listener on the scene for when it needs to switch to another
    /*Callback c = std::bind( &RootCanal::OnSceneChange, this, std::placeholders::_1 );
    currScene->addEventListener( SceneEvent::CHANGE, c  );*/
}

#pragma endregion