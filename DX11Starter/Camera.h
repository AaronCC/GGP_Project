#pragma once
#include "DXCore.h"
#include <DirectXMath.h>


class Camera
{
public:
	Camera();
	~Camera();

	DirectX::XMFLOAT3 camPos;
	DirectX::XMFLOAT3 camDir;
	DirectX::XMFLOAT3 forward;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 right;
	DirectX::XMFLOAT3 u;

	float xRot;
	float yRot;

	DirectX::XMFLOAT4X4 viewMat;
	DirectX::XMFLOAT4X4 projectionMatrix;

	DirectX::XMFLOAT4X4 GetViewMat();

	void Update(float deltaTime);
	void ProjectMat(unsigned int _w, unsigned int _h);
	void SetRot(float _xrot, float _yrot);

	DirectX::XMFLOAT4X4 GetProjectionMatrix();
};

