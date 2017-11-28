#include "Enemy.h"


Enemy::Enemy(Materials * material, ID3D11Device * device, XMFLOAT3 pos)
{
	//load enemy model
	mesh = new Mesh("Assets/Models/enemy.obj", device);
	entity = new Entity(mesh, material);

	this->pos = pos;
	entity->SetTranslation(pos.x, pos.y, pos.z);
	depth = pos.z;

	//update this enemy
	entity->UpdateMatrix();
}


Enemy::~Enemy()
{
	delete mesh;
	delete entity;
}

void Enemy::Update(float deltaTime, float totalTime)
{
	//hardcoded speed for now
	const int speed = 5;

	//move up the lane
	depth -= deltaTime * speed;
	entity->SetTranslation(pos.x, pos.y, depth);
	entity->UpdateMatrix();
}
