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

	void CreateLevel(const UINT stage, const float variance, const float depth, const float length);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	UINT stage;

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

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	Player* player;

	Camera* Cam;

	Materials* checker_mat; //checker
	Materials* rainbow_mat; //rainbow
	Materials* level_mat; //level

	DirectionalLight light;
	DirectionalLight light2;

	PointLight pointLight1;


	ID3D11ShaderResourceView* checkerSRV;
	ID3D11ShaderResourceView* rainbowSRV;
	ID3D11ShaderResourceView* levelSRV;

	ID3D11SamplerState* sampleState;

	Level* level;
	//Entity* backDrop;

	float CA_Intensity;

	// Post process stuff
	//render trget view - draw to this
	ID3D11RenderTargetView* ppRTV;
	//shader rexource view - draw from this
	ID3D11ShaderResourceView* ppSRV;
	//vertex and pixel shader
	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;
};

