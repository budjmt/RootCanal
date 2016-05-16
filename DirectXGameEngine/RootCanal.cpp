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
#include "UIObject.h"

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
	for (auto m : Mesh::loadedMeshes )
		delete m.second;
	for (auto m : Material::loadedMaterials )
		delete m.second;
    for( auto t : Texture::loadedTextures )
        delete t.second;
    for( auto s : Shader::loadedShaders )
        delete s.second;

    delete currScene;
	delete post; delete postManager;
	ReleaseMacro(psamplerState);
	ReleaseMacro(ppRTV);
	ReleaseMacro(ppSRV);
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

	ZeroMemory(&d.rasterDesc, sizeof(d.rasterDesc));
	d.rasterDesc.FillMode = D3D11_FILL_SOLID;
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
	UIObject::loadFonts();
	CreateGeometry();
	CreateMatrices();

	setupPostProcess();
	ID3D11SamplerState* samplerState;

	// Create the sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &samplerState);


	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    SetScene( new MenuScene( &camera ) );

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
    Shader::createShader<SimpleVertexShader>
    (
        L"BasicVertex",
        L"VertexShader.cso",
        device,
        deviceContext
    );

    Shader::createShader<SimplePixelShader>
    (
        L"BasicPixel",
        L"PixelShader.cso",
        device,
        deviceContext
     );

    Shader::createShader<SimplePixelShader>
    (
        L"ToonPixel",
        L"ToonPixelShader.cso",
        device,
        deviceContext
    );

	Shader::createShader<SimplePixelShader>
	(
		L"ToothPixel",
		L"ToothPixel.cso",
		device,
		deviceContext
	);

	Shader::createShader<SimpleVertexShader>
	(
		L"FinalVertex",
		L"FinalVS.cso",
		device,
		deviceContext
	);
	Shader::createShader<SimplePixelShader>
		(
			L"FinalPixel",
			L"Final.cso",
			device,
			deviceContext
			);

	Shader::createShader<SimplePixelShader>
	(
		L"OpacityMap",
		L"OpacityPost.cso",
		device,
		deviceContext
	);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void RootCanal::CreateGeometry()
{
#if DEBUG
	DrawDebug::getInstance().camera(&camera);
#endif
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void RootCanal::CreateMatrices()
{
    // TODO: Figure out why the camera cannot be deleted here when necessary
    //if( camera ) delete camera;

    // TODO: Clean this up and everything involving cameras and transforms
    camera = new Camera();
	camera->transform.position( vec3(0.f, 0.f, -5.f) );
	camera->update(0.f, &mouse);
	camera->windowWidth = windowWidth; camera->windowHeight = windowHeight;
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
	if (camera) {
		camera->windowWidth = windowWidth; camera->windowHeight = windowHeight;
		camera->updateProjection(windowWidth, windowHeight, aspectRatio);
	}
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

    // If the game is not in a default scene state and needs to change, then
    // change to the appropriate scene
    SceneType sceneType = StateManager::getInstance().changeScene();
    if( sceneType != SceneType::DEFAULT )
    {
        CreateMatrices();

        switch( sceneType )
        {
        case SceneType::MENU:
            SetScene( new MenuScene( &camera ) );
            break;

        case SceneType::GAME:
            SetScene( new GameScene( &camera ) );
            break;

        case SceneType::GAME_OVER:
            SetScene( new GameOverScene( &camera ) );
            break;
        }

        OnResize();
        StateManager::getInstance().changeScene( SceneType::DEFAULT );
    }

    StateManager::getInstance().update( deltaTime, &mouse );

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
	//deviceContext->ClearRenderTargetView(renderTargetView, color);

	deviceContext->OMSetRenderTargets(1, &ppRTV, depthStencilView);
	deviceContext->ClearRenderTargetView(ppRTV, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	1.0f, 0);

    currScene->draw();
	post->draw(ppSRV,ppRTV,renderTargetView);

#if DEBUG
    //DrawDebug::getInstance().draw();
#endif

    // Present the buffer
    //  - Puts the image we're drawing into the window so the user can see it
    //  - Do this exactly ONCE PER FRAME
    //  - Always at the very end of the frame
    if( swapChain )
        HR( swapChain->Present( 0, 0 ) );
}

void RootCanal::SetScene( Scene* scene )
{
    post->setSceneType( StateManager::getInstance().changeScene() );

    if( currScene ) delete currScene;
    currScene = scene;
}

void RootCanal::setupPostProcess() {
	// Create a texture
	D3D11_TEXTURE2D_DESC tDesc;
	tDesc.Width = windowWidth; tDesc.Height = windowHeight;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.MipLevels = 1; tDesc.MiscFlags = 0;
	tDesc.SampleDesc.Count = 1; tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* ppTexture;
	device->CreateTexture2D(&tDesc, 0, &ppTexture);

	// Make a render target view for rendering into the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = tDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(ppTexture, &rtvDesc, &ppRTV);

	// Make an SRV 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = tDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(ppTexture, &srvDesc, &ppSRV);

	// Get rid of ONE of the texture references
	//ppTexture->Release();
	ReleaseMacro(ppTexture);

	// Create the sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &psamplerState);

	post = new PostProcess(device, deviceContext, psamplerState, depthStencilView);
	postManager = new PostChainManager(post, windowWidth, windowHeight, device, deviceContext, psamplerState);
}

#pragma endregion