#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Materials.h"
#include "Vertex.h"
#include "Light.h"
#include "WICTextureLoader.h"
#include "Level.h"
#include "Player.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include <stdio.h>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	void CreateLevel(const UINT stage, const float variance, const float depth, const float length, int maxEnemies);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	int state; // 0 menu, 1 game
	UINT stage;
	int score;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Font Stuff
	DirectX::SpriteBatch* spriteBatch;
	DirectX::SpriteFont* font;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	Player* player;

	Camera* Cam;

	Materials* particle_mat; //checker
	Materials* checker_mat; //checker
	Materials* rainbow_mat; //rainbow
	Materials* level_mat; //level
	Materials* outline_mat; //outline
	Materials* skyPlane_mat; //skyPlane
	Materials* wheel_mat;

	DirectionalLight light;
	DirectionalLight light2;

	PointLight pointLight1;
	PointLight pointLight2;

	//skyplane lights
	PointLight skyLight1;
	//PointLight skyLight2;
	//PointLight skyLight3;

	ID3D11ShaderResourceView* particleSRV;
	ID3D11ShaderResourceView* checkerSRV;
	ID3D11ShaderResourceView* rainbowSRV;
	ID3D11ShaderResourceView* levelSRV;
	ID3D11ShaderResourceView* outlineSRV;
	ID3D11ShaderResourceView* skyPlaneSRV;
	ID3D11ShaderResourceView* wheelSRV;

	ID3D11SamplerState* sampleState;

	Level* level;
	//Entity* backDrop;

	// Post process stuff
	//render target view - draw to this
	ID3D11RenderTargetView* ppRTV;
	//shader rexource view - draw from this
	ID3D11ShaderResourceView* ppSRV;
	//vertex and pixel shader for chromactic aberration
	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;


	//render target view - draw to this
	ID3D11RenderTargetView* ABloom_RTV;
	ID3D11RenderTargetView* BBloom_RTV;
	//shader rexource view - draw from this
	ID3D11ShaderResourceView* ABloom_SRV;
	ID3D11ShaderResourceView* BBloom_SRV;
	//vs and ps for bloom
	SimpleVertexShader* BloomVS;
	SimplePixelShader* BloomPS;

	//outline stuff
	ID3D11RasterizerState* invRasterState;

	//blend stuff
	ID3D11BlendState* blendState;

	//SkyPlane stuff
	Mesh* skyPlaneMesh;
	Entity* skyPlaneEntity;

	Mesh* wheelMesh;
	Entity* wheelEntity;
};

