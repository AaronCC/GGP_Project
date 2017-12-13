#include "Projectile.h"

using namespace DirectX;

Projectile::Projectile(Materials * material, ID3D11Device * device, XMFLOAT3 pos)
{
	Vertex vertices[] =		//(pos)(norm)(uv)
	{
		{ XMFLOAT3(-1.25f, +1.25f, 0), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }, //Top Left
		{ XMFLOAT3(+1.25f, +1.25f, 0), XMFLOAT3(0,0,-1), XMFLOAT2(1,0) }, //Top Right
		{ XMFLOAT3(+1.25f, -1.25f, 0), XMFLOAT3(0,0,-1), XMFLOAT2(1,1) }, //Bot Right
		{ XMFLOAT3(-1.25f, -1.25f, 0), XMFLOAT3(0,0,-1), XMFLOAT2(0,1) }  //Bot Left
	};

	int indices[] = { 0,1,2,2,3,0 };

	mesh = new Mesh(vertices, 4, indices, 6, device);

	//load the projectile model
	//mesh = new Mesh("Assets/Models/sphere.obj", device);

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
