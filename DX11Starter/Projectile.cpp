#include "Projectile.h"

using namespace DirectX;

Projectile::Projectile(Materials * material, ID3D11Device * device, XMFLOAT3 pos)
{
	//load the projectile model
	mesh = new Mesh("Assets/Models/sphere.obj", device);
	entity = new Entity(mesh, material);

	this->pos = pos;
	entity->SetTranslation(pos.x, pos.y, pos.z);
	depth = pos.z;

	//update this projectile
	entity->UpdateMatrix();
}


Projectile::~Projectile()
{
	delete mesh;
	delete entity;
}

void Projectile::Update(float deltaTime, float totalTime)
{
	//hardcoded speed for now
	const int speed = 45;

	//move down the lane
	depth += deltaTime * speed;
	entity->SetTranslation(pos.x, pos.y, depth);
	entity->UpdateMatrix();
}
