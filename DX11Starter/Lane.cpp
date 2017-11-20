#include "Lane.h"

Lane::Lane(XMFLOAT2 pos, float depth, Materials * enemyMat, Materials * projMat, ID3D11Device * device)
{
	this->enemyMat = enemyMat;
	this->projMat = projMat;
	this->device = device;
	this->pos = pos;
	this->depth = depth;

	this->doAberrate = false;
	this->aberrateTimer = 0.0;
}

Lane::~Lane()
{
	for each (Enemy* enemy in enemies)
		delete enemy;
	for each (Projectile* proj in projs)
		delete proj;
}

void Lane::SpawnEnemy()
{
	//add enemy to vector of enemies
	enemies.push_back(new Enemy(enemyMat, device, XMFLOAT3{ this->pos.x, this->pos.y, depth }));
}

void Lane::SpawnProj()
{
	//add projectile to vector of projectiles
	projs.push_back(new Projectile(projMat, device, XMFLOAT3{ this->pos.x, this->pos.y, 0.f }));
}

void Lane::Update(float deltaTime, float totalTime, float random)
{
	/*std::vector<int> todelete;
	int delIndex;*/

	//spawn enemies
	if (random == 1)
	{
		SpawnEnemy();
	}

	//update each enemy in lane
	for each(Enemy* enemy in enemies)
	{
		enemy->Update(deltaTime, totalTime);
		/*delIndex++;
		if (enemy->getDepth() < 0)
			todelete.push_back(delIndex);*/
	}
	/*for (int i = 0; i < todelete.size(); i++)
	{
		if (i < todelete.size() - 1)
			todelete[i + 1] -= i + 1;
		enemies.erase(enemies.begin() + i);
	}*/

	/*todelete.clear();
	delIndex = 0;*/

	//update each projectile in lane
	for each(Projectile* proj in projs)
	{
		proj->Update(deltaTime, totalTime);
		/*delIndex++;
		if (proj->getDepth() < 0)
			todelete.push_back(delIndex);*/
	}
	/*for (int i = 0; i < todelete.size(); i++)
	{
		if (i < todelete.size() - 1)
			todelete[i + 1] -= i + 1;
		projs.erase(projs.begin() + i);
	}*/

	//set bounds for enemies and projectiles
	Enemy* enemy = nullptr;
	Projectile* proj = nullptr;
	if (projs.size() > 0) {
		proj = projs[0];
		if (proj->getDepth() > this->depth) { projs.erase(projs.begin());
		delete proj;
		}
	}
	if (enemies.size() > 0) {
		enemy = enemies[0];
		if (enemy->getDepth() < 0) { enemies.erase(enemies.begin());
			delete enemy;
			
			aberrateTimer += 1.0; // when an enemy reaches the end of the level, increase the chro abb time
			if (aberrateTimer >= 3.0) // cap the timer
				aberrateTimer = 3.0;
		}
	}

	//if the timer is non-zero, turn abberation on and decrement the timer
	if(aberrateTimer > 0.0) {
		doAberrate = true;
		aberrateTimer -= deltaTime;
	}
	else { //if the timer is zero, turn aberration off
		doAberrate = false;
	}

	//bargain bin collision
	if (enemy != nullptr && proj != nullptr) {
		// HARDCODED FOR NOW
		if (proj->getDepth() + 2 >= enemy->getDepth())
		{
			projs.erase(projs.begin());
			enemies.erase(enemies.begin());
			delete proj;
			delete enemy;
		}
	}

}