#pragma once
#include "Projectile.h"
#include "Enemy.h"
#include "Vertex.h"
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Lane
{
public:
	Lane(XMFLOAT2 pos, float depth);
	~Lane();

	void SpawnEnemy();
	void SpawnProj();
	void Update();

private:
	XMFLOAT2 pos; // X and Y of the lane
	float depth;
	std::vector<Projectile*> projs;
	std::vector<Enemy*> enemies;
};

