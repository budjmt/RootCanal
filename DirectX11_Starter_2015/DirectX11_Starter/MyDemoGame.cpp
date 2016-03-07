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

#include "MyDemoGame.h"
#include <time.h>

#include "ModelHelper.h"
#include "DrawDebug.h"

// For the DirectX Math library
using namespace DirectX;


#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the game object.
	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"My Super Fancy GGP Game";

	// Custom window size - will be created by Init() later
	windowWidth = 1280;
	windowHeight = 720;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
	delete vertexShader; delete pixelShader;
	for (Mesh* m : meshes)
		delete m;
	for (Material* m : materials)
		delete m;
	for (Entity* e : entities)
		delete e;
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	DXInfo& d = DXInfo::getInstance();
	d.device = device;
	d.deviceContext = deviceContext;
	deviceContext->RSGetState(&d.rasterState);
	if (d.rasterState)
		d.rasterState->GetDesc(&d.rasterDesc);
	d.swapChain = swapChain;
	d.depthStencilBuffer = depthStencilBuffer;
	d.renderTargetView = renderTargetView;
	d.depthStencilView = depthStencilView;
	d.viewport = &viewport;
	d.driverType = &driverType;
	d.featureLevel = &featureLevel;

	srand((uint32_t)time(NULL));

	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	LoadShaders(); 
	CreateGeometry();
	CreateMatrices();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Successfully initialized
	return true;
}


// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	camera = new Camera();
	OnResize();
	entities.push_back(camera);

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

	char* m_names[] = { "Assets/given/cone.obj", "Assets/given/cube.obj", "Assets/given/cylinder.obj"/*, "Assets/given/helix.obj", "Assets/given/sphere.obj", "Assets/given/torus.obj"*/ };
	Material* mat = new Material(); mat->vertexShader(vertexShader); mat->pixelShader(pixelShader); mat->camera(&camera);
	mat->texture(Texture::getTexture(L"Assets/texture.png", device, deviceContext));
	materials.push_back(mat);
	int i = 0;
	for (char* file : m_names) {
		Mesh* loadedMesh = loadOBJ(file);
		assert(loadedMesh != nullptr);
		DrawMesh* d = new DrawMesh(loadedMesh, nullptr, device);
		d->material(mat);
		vec3 vecs[3];
		float rot = rand() % 360 / 180.f * PI;
		//vecs[0] = vec3((rand() % 2 * 2 - 1) * (rand() % 1000 / 500.f), (rand() % 2 * 2 - 1) * (rand() % 1000 / 500.f), (rand() % 2 * 2 - 1) * (rand() % 1000 / 500.f));
		//vecs[1] = vec3((rand() % 1000 / 500.f), (rand() % 1000 / 500.f), (rand() % 1000 / 500.f));
		vecs[2] = vec3((rand() % 2 * 2 - 1) * (rand() % 1000 / 1000.f), (rand() % 2 * 2 - 1) * (rand() % 1000 / 1000.f), (rand() % 2 * 2 - 1) * (rand() % 1000 / 1000.f));
		vecs[2] /= vec3::length(vecs[2]);
		vecs[0] = vec3(8.f - (i + 1) * 4.f, 0.f, 0.f);
		vecs[1] = vec3((i + 1) * 0.5f, (i + 1) * 0.5f, (i + 1) * 0.5f);
		//vecs[2] = vec3(1,0,0);
		Entity* e = new Entity(vecs[0], vecs[1], vecs[2], rot, d);
		meshes.push_back(loadedMesh);
		entities.push_back(e);
		i++;
	}
}


// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void MyDemoGame::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//   update when/if the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	/*XMMATRIX W, T = XMMatrixTranslation(.5f, .75f, .5f), R = XMMatrixRotationY(-PI / 4), S = XMMatrixScaling(1.5f, 1.f, 1.f);
	mat4 w, t = mat4::translate(vec3(0.5f, 0.75f, 0.5f)), r = mat4::rotate(PI / 4, vec3(0,1,0)), s = mat4::scale(vec3(1.5f,1.f,1.f));
	W = XMMatrixMultiply(T, R); W = XMMatrixMultiply(W, S);
	w = t * r * s;*/
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!
    
	camera->transform.position = vec3(0.f, 0.f, -5.f);
	//camera->transform.rotate(0, PI, 0);
	camera->update(0.f, &mouse);
	camera->updateProjection(windowWidth, windowHeight, aspectRatio);

	// Create the View matrix
	// - In an actual game, recreate this matrix when the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction you want it to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up  = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V   = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		aspectRatio,		  	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	
	if (camera) camera->updateProjection(windowWidth, windowHeight, aspectRatio);
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
float x = 0;
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	
	Camera::mayaCam(windowWidth, windowHeight, deltaTime, &mouse, camera);
	for (auto e : entities)
		e->update(deltaTime, &mouse);

	mouse.prev.x = mouse.curr.x;
	mouse.prev.y = mouse.curr.y;
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("inv_trans_world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);
	camera->updateCamMat(vertexShader);

	pixelShader->SetData("light1", &l1, sizeof(DirectionalLight));
	pixelShader->SetData("light2", &l2, sizeof(DirectionalLight));
	
	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader(true);
	pixelShader->SetShader(true);
	
	for (Entity* e : entities) {
		e->draw(deviceContext);
	}
	#if DEBUG
		//DrawDebug::getInstance().drawDebugSphere(vec3(),0.5f);
		//DrawDebug::getInstance().drawDebugVector(vec3(), vec3(1,1,1));
		DrawDebug::getInstance().draw();
	#endif

	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	mouse.btnState = btnState;
	//mouse.prev.x = mouse.curr.x;
	//mouse.prev.y = mouse.curr.y;
	mouse.curr.x = x;
	mouse.curr.y = y;
	mouse.lastPress = GetTickCount64();
	mouse.down = true;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	mouse.btnState = btnState;
	mouse.down = false;

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	mouse.btnState = btnState;
	//mouse.prev.x = mouse.curr.x;
	//mouse.prev.y = mouse.curr.y;
	mouse.curr.x = x;
	mouse.curr.y = y;
}
#pragma endregion