#include "Game.h"

#define MAX_LEVEL_INDEX 3

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		800,			   // Width of the window's client area
		800,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

	Cam = new Camera();

	skyPlaneMesh = 0;
	skyPlaneEntity = 0;

	horz1 = 0;
	vert1 = 10;
	horz2 = -10;
	vert2 = 0;
	horz3 = 10;
	vert3 = -10;
	deep = 50;


#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff

	delete vertexShader;
	delete pixelShader;
	delete Cam;
	delete particle_mat;
	delete checker_mat;
	delete rainbow_mat;
	delete level_mat;
	delete outline_mat;
	delete skyPlane_mat;

	//Clean up the skyplane
	delete skyPlaneMesh;
	delete skyPlaneEntity;

	//Level1->~Level();
	delete level;
	delete player;

	particleSRV->Release();
	checkerSRV->Release();
	rainbowSRV->Release();
	levelSRV->Release();
	outlineSRV->Release();
	sampleState->Release();
	skyPlaneSRV->Release();

	//clean up post process stuff
	ppSRV->Release();
	ppRTV->Release();
	delete ppPS;
	delete ppVS;

	ABloom_SRV->Release();
	ABloom_RTV->Release();
	BBloom_SRV->Release();
	BBloom_RTV->Release();
	delete BloomPS;
	delete BloomVS;

	//clean up outline stuff
	invRasterState->Release();

	//clean up blend
	blendState->Release();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry(); //needed for skyplane

	light.AmbientColor = XMFLOAT4(0.0, 0.0, 0.0, 1.0);
	light.DiffuseColor = XMFLOAT4(1, 0, 0, 1);
	light.Direction = XMFLOAT3(1, -1, 0);

	light2.AmbientColor = XMFLOAT4(0.0, 0.0, 0.0, 1.0);
	light2.DiffuseColor = XMFLOAT4(1, 0, 0, 1);
	light2.Direction = XMFLOAT3(0.5, 0, 1);

	pointLight1.PL_Position = XMFLOAT3(0, 0, 32.5); //put the light at depth/2
	pointLight1.PL_Color = XMFLOAT4(1, 1, 1, 1);

	pointLight2.PL_Position = XMFLOAT3(0, 0, -20); //put the light at origin
	pointLight2.PL_Color = XMFLOAT4(1, 1, 1, 1);

	skyLight1.PL_Position = XMFLOAT3(horz1, vert1, deep);
	skyLight1.PL_Color = XMFLOAT4(1, 0, 0, 1);

	skyLight2.PL_Position = XMFLOAT3(horz2, vert2, deep);
	skyLight2.PL_Color = XMFLOAT4(0, 1, 0, 1);

	skyLight3.PL_Position = XMFLOAT3(horz3, vert3, deep);
	skyLight3.PL_Color = XMFLOAT4(0, 0, 1, 1);


	//create the level
	//level = new Level(level_mat);

	////set the level variables
	//const int LANES = 10; // the uv mapping does not like odd numbers
	//Vertex verts[LANES * 2] = {}; //vertex array null w/ length = lanecount*2
	//int inds[LANES * 6] = {}; // ind array w/ length = lanecount*6

	////generate the level
	//level->genLevel(device,		// device
	//	inds,		// array of indices
	//	verts,		// vertex of verts
	//	LANES,		// number of lanes
	//	8.0,		// starting distance of vertex to origin
	//	8,			// max variance
	//	75.0,		// depth of level
	//	checker_mat,	// enemy material
	//	rainbow_mat);	// projectile material

	level = nullptr;
	stage = 1;
	CreateLevel(this->stage, 8.f, 75.f, 8.f, 4);

	// Create Player
	this->player = new Player(level, rainbow_mat, outline_mat, device);

	//create backdrop
	//backDrop = new Entity();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	// Post processing shaders
	ppVS = new SimpleVertexShader(device, context);
	ppVS->LoadShaderFile(L"PP_VS.cso");

	ppPS = new SimplePixelShader(device, context);
	ppPS->LoadShaderFile(L"PP_PS.cso");

	//bloom post process shaders
	BloomVS = new SimpleVertexShader(device, context);
	BloomVS->LoadShaderFile(L"Bloom_VS.cso");

	BloomPS = new SimplePixelShader(device, context);
	BloomPS->LoadShaderFile(L"Bloom_PS.cso");

	CreateWICTextureFromFile(device, context, L"Assets/Textures/particle_texture.png", 0, &particleSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/checker.png", 0, &checkerSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/rainbow_texture.png", 0, &rainbowSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/level.png", 0, &levelSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/gold.png", 0, &outlineSRV);
	CreateWICTextureFromFile(device, context, L"Assets/Textures/starBG.gif", 0, &skyPlaneSRV);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampleState);

	particle_mat = new Materials(pixelShader, vertexShader, particleSRV, sampleState);
	checker_mat = new Materials(pixelShader, vertexShader, checkerSRV, sampleState);
	rainbow_mat = new Materials(pixelShader, vertexShader, rainbowSRV, sampleState);
	level_mat = new Materials(pixelShader, vertexShader, levelSRV, sampleState);
	outline_mat = new Materials(pixelShader, vertexShader, outlineSRV, sampleState);
	skyPlane_mat = new Materials(pixelShader, vertexShader, skyPlaneSRV, sampleState);

	// set up post processing resources
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	//the texture to render to
	ID3D11Texture2D* ppTexture;
	device->CreateTexture2D(&textureDesc, 0, &ppTexture);

	// Create the Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = textureDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	device->CreateRenderTargetView(ppTexture, &rtvDesc, &ppRTV);

	//create 2 tex2d's for bloom post process
	ID3D11Texture2D* bloomTextureA;
	device->CreateTexture2D(&textureDesc, 0, &bloomTextureA);
	device->CreateRenderTargetView(bloomTextureA, &rtvDesc, &ABloom_RTV);

	ID3D11Texture2D* bloomTextureB;
	device->CreateTexture2D(&textureDesc, 0, &bloomTextureB);
	device->CreateRenderTargetView(bloomTextureB, &rtvDesc, &BBloom_RTV);


	// Create the Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	device->CreateShaderResourceView(ppTexture, &srvDesc, &ppSRV);

	//srv's for bloom
	device->CreateShaderResourceView(bloomTextureA, &srvDesc, &ABloom_SRV);
	device->CreateShaderResourceView(bloomTextureB, &srvDesc, &BBloom_SRV);

	//create inverted rasterizer state
	D3D11_RASTERIZER_DESC rasDesc = {};
	rasDesc.FillMode = D3D11_FILL_SOLID;	// draw solid
	rasDesc.CullMode = D3D11_CULL_FRONT;	// draw the inside faces
	rasDesc.DepthClipEnable = true;

	device->CreateRasterizerState(&rasDesc, &invRasterState);

	//create blend description
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&bd, &blendState);

	//float factors[4] = { 1,1,1,1 };
	//context->OMSetBlendState(blendState, factors, 0xFFFFFFFF);

	//release the texture because it is now useless
	ppTexture->Release();
	//bloom
	bloomTextureA->Release();
	bloomTextureB->Release();
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	Cam->ProjectMat(width, height);
}


// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	Vertex vertices[] =		//(pos)(norm)(uv)
	{
		{ XMFLOAT3(-50.0f, +50.0f, +75.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }, //Top Left
		{ XMFLOAT3(+50.0f, +50.0f, +75.0f), XMFLOAT3(0,0,-1), XMFLOAT2(1,0) }, //Top Right
		{ XMFLOAT3(+50.0f, -50.0f, +75.0f), XMFLOAT3(0,0,-1), XMFLOAT2(1,1) }, //Bot Right
		{ XMFLOAT3(-50.0f, -50.0f, +75.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,1) }  //Bot Left
	};

	int indices[] = { 0,1,2,2,3,0 };

	skyPlaneMesh = new Mesh(vertices, 4, indices, 6, device);
	skyPlaneEntity = new Entity(skyPlaneMesh, skyPlane_mat); ///////////////////////////////////////////////

}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	Cam->ProjectMat(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	if (level->getLevelClear() == true) {
		this->stage++;
		CreateLevel(this->stage, 8.f, 75.f, 8.f, 5);
		player->setLevel(this->level);
		//set the player's position to 0
		player->setPos(0);
	}

	//timer for some of the transformations
	float sinTime = (sin(totalTime * 10.0f) + 2.0f) / 10.0f;

	Cam->Update(deltaTime);

	//update game objects
	level->Update(deltaTime, totalTime);
	player->Update(deltaTime, totalTime);
	skyPlaneEntity->UpdateMatrix();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	//const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	const float color[4] = { 0.8f, 0.8f, 0.8f, 0.0f }; //new grey color

	//turn off all srv's to avoid input/output clashing
	ID3D11ShaderResourceView* nullSRVs[16] = {};
	context->PSSetShaderResources(0, 16, nullSRVs);

	//first draw everything to ppRTV
	//post processing - swap render target to texture2d
	context->OMSetRenderTargets(1, &ppRTV, depthStencilView);


	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(ppRTV, color);
	context->ClearRenderTargetView(ABloom_RTV, color);
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	//pixelShader->SetData(
	//	"light",	//Name of the (eventual) variable in the shader 
	//	&light,		//address of the data to copy
	//	sizeof(DirectionalLight));	//size of data to copy

	//pixelShader->SetData(
	//	"light2",	//Name of the (eventual) variable in the shader 
	//	&light2,		//address of the data to copy
	//	sizeof(DirectionalLight));	//size of data to copy

	//pass point light to the pixel shader
	pixelShader->SetData(
		"pointLight1",
		&pointLight1,
		sizeof(PointLight));

	pixelShader->SetData(
		"pointLight2",
		&pointLight2,
		sizeof(PointLight));

	//skyLight stuff
	pixelShader->SetData(
		"skyLight1",
		&skyLight1,
		sizeof(PointLight));

	pixelShader->SetData(
		"skyLight2",
		&skyLight2,
		sizeof(PointLight));

	pixelShader->SetData(
		"skyLight3",
		&skyLight3,
		sizeof(PointLight));

	//pass the cameras position to the camera
	pixelShader->SetData(
		"CameraPosition",
		&Cam->camPos,
		sizeof(XMFLOAT3));

	pixelShader->CopyAllBufferData();

	//draw the skyPlane
	skyPlaneEntity->Draw(context, Cam->GetViewMat(), Cam->GetProjectionMatrix(),
	skyPlaneMesh, sampleState);

	//get vector of lanes
	std::vector<Lane*>* lanes = level->getLanes();
	float CroAbb = 0;
	//loop through lanes
	for each(Lane* lane in *lanes)
	{
		CroAbb += lane->getAberration();

		//draw each enemy
		for each(Enemy* enemy in *lane->getEnemies())
		{
			Entity* enemyEntity = enemy->getEntity();
			enemyEntity->Draw(context, Cam->GetViewMat(), Cam->GetProjectionMatrix(), enemyEntity->mesh, sampleState);
		}
		//draw each projectile
		for each(Projectile* proj in *lane->getProjectiles())
		{
			Entity* projEntity = proj->getEntity();
			projEntity->Draw(context, Cam->GetViewMat(), Cam->GetProjectionMatrix(), projEntity->mesh, sampleState);
		}
	}

	//draw the level
	float factors[4] = { 1,1,1,1 };
	context->OMSetBlendState(blendState, factors, 0xFFFFFFFF); // set blend state

	Entity* levelEntity = level->getEntity();
	levelEntity->Draw(context, Cam->GetViewMat(), Cam->GetProjectionMatrix(), levelEntity->mesh, sampleState);
	
	context->OMSetBlendState(0, factors, 0xFFFFFFFF);	//reset blendstate

	//draw the player
	Entity* playerEntity = player->getEntity();
	playerEntity->Draw(context, Cam->GetViewMat(), Cam->GetProjectionMatrix(), playerEntity->mesh, sampleState);

	//draw the player outline
	Entity* playerEntityOutline = player->getOutlineEntity();	//get the player entity - scaled up in player
	context->RSSetState(invRasterState);				//set render state to new rasterizer
	playerEntityOutline->Draw(context, Cam->GetViewMat(), Cam->GetProjectionMatrix(), playerEntityOutline->mesh, sampleState);
	context->RSSetState(0);	//reset renderstate


	context->PSSetShaderResources(0, 16, nullSRVs);

	//post processing
	//chromatic aberration
	//set render target to Bloom's rtv + clear it
	context->OMSetRenderTargets(1, &ABloom_RTV, 0);
	context->ClearRenderTargetView(ABloom_RTV, color);

	//draw with the post process shaders
	ppVS->SetShader();
	ppPS->SetShader();

	//send in chromatic aberration value
	ppPS->SetFloat("SplitIntensity", CroAbb);
	ppPS->CopyAllBufferData();

	ppPS->SetShaderResourceView("Pixels", ppSRV);
	ppPS->SetSamplerState("Sampler", sampleState);


	//disable vertex and index buffer
	ID3D11Buffer* nullBuffer = 0;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	//draw 3 verts - the triangle that will cover the screen
	context->Draw(3, 0);


	context->PSSetShaderResources(0, 16, nullSRVs);


	//now bloom
	int BlurAmount = 12;

	//first step = extract bright pixels from ABloomRTV into BBloomRTV
	//set render target to BBloomRTV + clear it
	context->OMSetRenderTargets(1, &BBloom_RTV, 0);
	context->ClearRenderTargetView(BBloom_RTV, color);

	////go down the bloom shader rabbit hole - first extract bright pixels
	BloomVS->SetShader();
	BloomPS->SetShader();

	//send in extra data to pixel shader here
	//let the bloom PS know which task it's on - task 1
	float PassButter = 0;
	BloomPS->SetFloat("Purpose", PassButter);

	//send in desaturation intensity 0-1
	BloomPS->SetFloat("DesatVal", 0.7);
	BloomPS->CopyAllBufferData();

	BloomPS->SetShaderResourceView("Pixels", ABloom_SRV);
	BloomPS->SetSamplerState("Sampler", sampleState);

	//disable vertex and index buffer
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	//draw 3 verts - the triangle that will cover the screen
	context->Draw(3, 0);


	context->PSSetShaderResources(0, 16, nullSRVs);



	//second step = blur horizontally
	//set render target to BBloomRTV + clear it
	context->OMSetRenderTargets(1, &ppRTV, 0);
	context->ClearRenderTargetView(ppRTV, color);

	//send in extra data to pixel shader here
	//let the bloom PS know which task it's on
	PassButter = 1;
	BloomPS->SetFloat("Purpose", PassButter);

	//set up for blur
	BloomPS->SetFloat("PixelWidth", 1.0f / width);
	//ppPS->SetFloat("pixelHeight", 1.0f / height);
	BloomPS->SetInt("BlurAmount", BlurAmount);

	BloomPS->CopyAllBufferData();

	BloomPS->SetShaderResourceView("Pixels", BBloom_SRV);

	//disable vertex and index buffer
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	//draw 3 verts - the triangle that will cover the screen
	context->Draw(3, 0);


	context->PSSetShaderResources(0, 16, nullSRVs);


	//third step = blur vertically
	//set render target to BBloom_RTV + clear it
	context->OMSetRenderTargets(1, &BBloom_RTV, 0);
	context->ClearRenderTargetView(BBloom_RTV, color);

	//send in extra data to pixel shader here
	//let the bloom PS know which task it's on
	PassButter = 2;
	BloomPS->SetFloat("Purpose", PassButter);

	//set up for blur
	BloomPS->SetFloat("PixelHeight", 1.0f / width);
	BloomPS->SetInt("BlurAmount", BlurAmount);

	BloomPS->CopyAllBufferData();

	BloomPS->SetShaderResourceView("Pixels", ppSRV);

	//disable vertex and index buffer
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	//draw 3 verts - the triangle that will cover the screen
	context->Draw(3, 0);


	context->PSSetShaderResources(0, 16, nullSRVs);


	//fourth step = combine
	//set render target to backbuffer + clear it
	context->OMSetRenderTargets(1, &backBufferRTV, 0);
	context->ClearRenderTargetView(backBufferRTV, color);

	//send in extra data to pixel shader here
	//let the bloom PS know which task it's on
	PassButter = 3;
	BloomPS->SetFloat("Purpose", PassButter);

	//set up for blur
	BloomPS->CopyAllBufferData();

	BloomPS->SetShaderResourceView("Pixels", BBloom_SRV);
	BloomPS->SetShaderResourceView("Pixels2", ABloom_SRV);

	//disable vertex and index buffer
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	//draw 3 verts - the triangle that will cover the screen
	context->Draw(3, 0);


	context->PSSetShaderResources(0, 16, nullSRVs);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}

void Game::CreateLevel(const UINT stage, const float variance, const float depth, const float length, int maxEnemies)
{
	if (level != nullptr) {
		//level->~Level();
		delete level;
	}

	if (stage <= MAX_LEVEL_INDEX)
		level = new Level(level_mat);

	if (stage == 1) {
		//remake the level
		//set the level variables
		const int LANES = 4; // the uv mapping does not like odd numbers
		Vertex verts[LANES * 2] = {}; //vertex array null w/ length = lanecount*2
		int inds[LANES * 6] = {}; // ind array w/ length = lanecount*6

		level->genLevel(device,		// device
			inds,		// array of indices
			verts,		// vertex of verts
			LANES,		// number of lanes
			length,		// starting distance of vertex to origin
			variance,			// max variance
			depth,
			maxEnemies,				// depth of level
			checker_mat,	// enemy material
			particle_mat);	// projectile material
	}
	else if (stage == 2) {
		const int LANES = 6; // the uv mapping does not like odd numbers
		Vertex verts[LANES * 2] = {}; //vertex array null w/ length = lanecount*2
		int inds[LANES * 6] = {}; // ind array w/ length = lanecount*6

		level->genLevel(device,		// device
			inds,		// array of indices
			verts,		// vertex of verts
			LANES,		// number of lanes
			length,		// starting distance of vertex to origin
			variance,			// max variance
			depth,
			maxEnemies,		// depth of level
			checker_mat,	// enemy material
			particle_mat);	// projectile material
	}
	else if (stage == 3) {
		const int LANES = 8; // the uv mapping does not like odd numbers
		Vertex verts[LANES * 2] = {}; //vertex array null w/ length = lanecount*2
		int inds[LANES * 6] = {}; // ind array w/ length = lanecount*6

		level->genLevel(device,		// device
			inds,		// array of indices
			verts,		// vertex of verts
			LANES,		// number of lanes
			length,		// starting distance of vertex to origin
			variance,			// max variance
			depth,
			maxEnemies,		// depth of level
			checker_mat,	// enemy material
			particle_mat);	// projectile material
	}
	else {
		level = nullptr;
		this->stage = 1;
		CreateLevel(this->stage, variance, depth, length, maxEnemies);
	}
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	Cam->SetRot(0, 0); //stop mouse movement when the button is released


	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	if (buttonState & 0x0001) {
		Cam->SetRot(y - prevMousePos.y, x - prevMousePos.x); //only moves the camera while the mouse button is pressed
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	bool dir = 0;
	if (wheelDelta < 0)
		dir = 0;
	else
		dir = 1;
	// Add any custom code here...
	player->WheelMove(dir);
}
#pragma endregion