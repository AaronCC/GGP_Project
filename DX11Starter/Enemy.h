#pragma once
#include "Mesh.h"
#include <DirectXMath.h>
#include "Materials.h"
#include "DXCore.h"
#include "Entity.h"

using namespace DirectX;

class Enemy
{
public:
	Enemy(Materials* material, ID3D11Device* device, XMFLOAT3 pos);
	~Enemy();

	void Update(float deltaTime, float totalTime);
	float getDepth() { return depth; }
	Entity* getEntity() { return entity; }

private:
	Mesh* mesh;
	Entity* entity;
	XMFLOAT3 pos;
	float depth;
};

