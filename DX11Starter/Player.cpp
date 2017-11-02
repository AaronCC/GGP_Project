#include "Player.h"

using namespace DirectX;

Player::Player(Level * level, Materials* material, ID3D11Device* device)
{
	this->level = level;
	mesh = new Mesh("Assets/Models/cube.obj", device);
	entity = new Entity(mesh, material);

	pos = 0.f;

	XMFLOAT3 loc = level->getLanePos(pos);
	entity->SetTranslation(loc.x, loc.y, 0.f);
}

Player::~Player()
{
	delete mesh;
	delete entity;
}

void Player::Update()
{
	//hotkeys
	if (oldAState == false && GetAsyncKeyState('A') & 0x8000) { 
		if (pos - 1 < 0)
			pos = level->getLaneCount() - 1;
		else
			pos--;
	}
	if (oldDState == false && GetAsyncKeyState('D') & 0x8000) {
		if (pos + 1 > level->getLaneCount() - 1)
			pos = 0;
		else
			pos++;
	}
	if (oldWState == false && GetAsyncKeyState('W') & 0x8000) {
		level->getLane(pos)->SpawnProj();
	}
	//track old key state
	oldAState = GetAsyncKeyState('A');
	oldDState = GetAsyncKeyState('D');
	oldWState = GetAsyncKeyState('W');

	//update player location
	XMFLOAT3 loc = level->getLanePos(pos);
	entity->SetTranslation(loc.x, loc.y, 0.f);
	entity->UpdateMatrix();
}
