#include "Entity.h"

using namespace DirectX;

Entity::Entity(Mesh* _mesh, Materials* _material)
{
	XMMATRIX ident = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMat, ident);

	position = XMFLOAT3(0,0,0);
	rotation = XMFLOAT3(0,0,0);
	scale = XMFLOAT3 (1,1,1);

	material = _material;

}


Entity::~Entity()
{
}

DirectX::XMFLOAT4X4 Entity::GetWorldMat() 
{
	return worldMat;
}

DirectX::XMFLOAT3 Entity::GetPosition() 
{
	return position;
}

DirectX::XMFLOAT3 Entity::GetRotation() 
{
	return rotation;
}

DirectX::XMFLOAT3 Entity::GetScale() 
{
	return scale;
}

void Entity::SetTranslation(float _x, float _y, float _z)
{
	position = XMFLOAT3(position.x + _x, position.y + _y, position.z + _z);
}

void Entity::SetRotation(float _x, float _y, float _z)
{
	rotation = XMFLOAT3(rotation.x + _x, rotation.y + _y, rotation.z + _z);
}

void Entity::SetScale(float _x, float _y, float _z)
{
	scale = XMFLOAT3(_x, _y, _z);
}

void Entity::UpdateMatrix()
{
	XMMATRIX posMat = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMMATRIX scaMat = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMStoreFloat4x4(&worldMat, XMMatrixTranspose(scaMat * rotMat * posMat));
}

void Entity::Draw(/*SimpleVertexShader* _vertShade, SimplePixelShader* _pixShade,*/  
	ID3D11DeviceContext* _context, DirectX::XMFLOAT4X4 _viewMatrix, DirectX::XMFLOAT4X4 _projectionMatrix, 
	Mesh* _shape, ID3D11SamplerState* _sample)
{
	material->GetVertShade()->SetMatrix4x4("world", GetWorldMat());
	material->GetVertShade()->SetMatrix4x4("view", _viewMatrix);
	material->GetVertShade()->SetMatrix4x4("projection", _projectionMatrix);
	material->GetPixShade()->SetSamplerState("basicSampler", _sample);
	material->GetPixShade()->SetShaderResourceView("diffuseTexture", material->GetSRV());

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	material->GetVertShade()->CopyAllBufferData();
	material->GetPixShade()->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	material->GetVertShade()->SetShader();
	material->GetPixShade()->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.

	//ONCE PER SHAPE
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ID3D11Buffer* buff = _shape->GetVertexBuffer();


	_context->IASetVertexBuffers(0, 1, &buff, &stride, &offset);
	_context->IASetIndexBuffer(_shape->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	_context->DrawIndexed(
		_shape->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
			   //ONCE PER SHAPE
}