#include "Camera.h"

using namespace DirectX;


Camera::Camera()
{
	camPos = XMFLOAT3(0.0f, 0.0f, -40.0f);
	camDir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	forward = XMFLOAT3(0.0f, 0.0f, 1.0f); //default forward vec
	up = XMFLOAT3(0.0f, 1.0f, 0.0f); //default up vec
	right = XMFLOAT3(1.0f, 0.0f, 0.0f);//default right vec
	u = XMFLOAT3(0.0f, 1.0f, 0.0f);//default up vec UNCHANGED

	xRot = 0;
	yRot = 0;

	XMMATRIX ident = XMMatrixIdentity();
	XMStoreFloat4x4(&viewMat, ident);
}


Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(xRot, yRot, 0.0f);//rotation matrix
	XMVECTOR vecCamDir = XMLoadFloat3(&camDir); //vector for camera direction
	vecCamDir = XMVector3Transform(vecCamDir, rotMat);//direction for view matrix
	
	XMVECTOR vecCamPos = XMLoadFloat3(&camPos);//vector for camera position
	XMVECTOR vecUp = XMLoadFloat3(&up);//up vector
	vecUp = XMVector3Transform(vecUp, rotMat);//'correct' up
	XMVECTOR vecRight = XMLoadFloat3(&right);//right vector
	vecRight = XMVector3Transform(vecRight, rotMat);//'correct' right

	XMVECTOR vecU = XMLoadFloat3(&u);//doesn't change
	
	//Keyboard Input
	//if (GetAsyncKeyState('W') & 0x8000) { vecCamPos += (vecCamDir * 10.1f *  deltaTime); }
	/*if (GetAsyncKeyState('S') & 0x8000) { vecCamPos += (vecCamDir * -10.1f *  deltaTime); }
	if (GetAsyncKeyState('D') & 0x8000) { vecCamPos += (vecRight * 10.1f *  deltaTime); }
	if (GetAsyncKeyState('A') & 0x8000) { vecCamPos += (vecRight * -10.1f *  deltaTime); }
	if (GetAsyncKeyState('E') & 0x8000) { vecCamPos += (vecUp * 10.1f *  deltaTime); }
	if (GetAsyncKeyState('Q') & 0x8000) { vecCamPos += (vecUp * -10.1f *  deltaTime); }*/
	

	//Final Sendoff
	XMMATRIX viewMatrix = XMMatrixLookToLH(vecCamPos, vecCamDir, vecU);//making the view matrix
	
	//End Steps
	XMStoreFloat4x4(&viewMat, XMMatrixTranspose(viewMatrix));//store final result
	XMStoreFloat3(&camDir, vecCamDir);//store final result
	XMStoreFloat3(&camPos, vecCamPos);//store final result
	XMStoreFloat3(&up, vecUp);//store final result
	XMStoreFloat3(&right, vecRight);//store final result
	//printf("%f, %f, %f\n", camPos.x, camPos.y, camPos.z);
}

DirectX::XMFLOAT4X4 Camera::GetViewMat()
{
	return viewMat;
}

void Camera::ProjectMat(unsigned int _w, unsigned int _h)
{
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)_w / _h,		// Aspect ratio
		0.1f,						// Near clip plane distance
		200.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::SetRot(float _xrot, float _yrot)
{
	xRot = (_xrot * 2 * 3.1415926535f) / 50000; //50000 pixels is a full 2 pi radians
	yRot = (_yrot * 2 * 3.1415926535f) / 50000; //50000 pixels is a full 2 pi radians
	//printf("%f , %f \n", xRot, yRot);
}
