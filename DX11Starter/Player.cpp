#include "Player.h"
#define COOLDOWN 1
using namespace DirectX;

Player::Player(Level * level, Materials* material, Materials* outlineMat, ID3D11Device* device)
{
	this->level = level;
	mesh = new Mesh("Assets/Models/player.obj", device);
	entity = new Entity(mesh, material);
	outlineEntity = new Entity(mesh, outlineMat);
	pos = 0.f;

	XMFLOAT3 loc = level->getLanePos(pos);
	entity->SetTranslation(loc.x, loc.y, 0.f);

	outlineEntity->SetTranslation(loc.x, loc.y, 0.f);
	outlineEntity->SetScale(1.5f, 1.5f, 1.5f);
	outlineEntity->UpdateMatrix();

	cooldown = COOLDOWN;
	ammo = 10;
}

Player::~Player()
{
	delete mesh;
	delete entity;
	delete outlineEntity;
}

// 0 = !clockwise, 1 = clockwise
void Player::WheelMove(bool dir)
{
	if(dir)
		if (pos + 1 > level->getLaneCount() - 1)
			pos = 0;
		else
			pos++;
	else
		if (pos - 1 < 0)
			pos = level->getLaneCount() - 1;
		else
			pos--;
}

void Player::Update(float deltaTime, float totalTime)
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
	if (oldWState == false && GetAsyncKeyState('W') & 0x8000 && ammo > 0/* && cooldown <= 0*/) {
		level->getLane(pos)->SpawnProj();
		ammo--;
		//cooldown = 0.5f; // set cooldown timer
	}

	//track and update cooldown of shooting
	cooldown -= deltaTime;
	if (cooldown <= 0) {
		cooldown = COOLDOWN;
		if (ammo < 10) {
			ammo++;
		}

	}

	//track old key state
	oldAState = GetAsyncKeyState('A');
	oldDState = GetAsyncKeyState('D');
	oldWState = GetAsyncKeyState('W');

	//update player location
	XMFLOAT3 loc = level->getLanePos(pos);
	entity->SetTranslation(loc.x, loc.y, 0.f);
	entity->UpdateMatrix();

	outlineEntity->SetTranslation(loc.x, loc.y, 0.f);
	outlineEntity->UpdateMatrix();
}
