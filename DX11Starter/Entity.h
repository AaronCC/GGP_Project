#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "Materials.h"

class Entity
{
public:
	Entity(Mesh* _mesh, Materials* _material);

	~Entity();

	DirectX::XMFLOAT4X4 GetWorldMat();
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMFLOAT3 GetScale();
	void SetTranslation(float _x, float _y, float _z);
	void SetRotation(float _x, float _y, float _z);
	void SetScale(float _x, float _y, float _z);
	void UpdateMatrix();
	void Draw(/*SimpleVertexShader* _vertShade, SimplePixelShader* _pixShade, */
		ID3D11DeviceContext* _context, DirectX::XMFLOAT4X4 _viewMatrix, DirectX::XMFLOAT4X4 _projectionMatrix, 
		Mesh* _shape, ID3D11SamplerState* _sample);

	Materials* material;

	Mesh* mesh;

private:
	DirectX::XMFLOAT4X4 worldMat;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	

};

