#pragma once
#include "Projectile.h"
#include "Enemy.h"
#include "Materials.h"
#include "Vertex.h"
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class Lane
{
public:

	Lane(XMFLOAT2 pos, float depth, int maxEnemies, Materials* enemyMat, Materials* projMat, ID3D11Device* device);
	~Lane();

	void SpawnEnemy();
	void SpawnProj();
	void Update(float deltaTime, float totalTime, float random);

	XMFLOAT3 getPos() { return XMFLOAT3{ pos.x, pos.y, depth }; }
	float getAberration() { return aberrateTimer; }
	std::vector<Enemy*>* getEnemies() { return &enemies; }
	std::vector<Projectile*>* getProjectiles() { return &projs; }

	int maxEnemies;
	int spawnedEnemies;
	int deadEnemies;
	bool clear;

private:
	XMFLOAT2 pos; // X and Y of the lane
	float depth;
	float aberrateTimer;
	float respawnTimer;



	std::vector<Projectile*> projs;
	std::vector<Enemy*> enemies;

	Materials* enemyMat;
	Materials* projMat;
	ID3D11Device* device;
};

