#pragma once
#include "Level.h"
#include "Mesh.h"
#include <DirectXMath.h>
#include "Materials.h"
#include "DXCore.h"

class Player
{
public:
	Player(Level * level, Materials * material, ID3D11Device * device);
	~Player();

	void WheelMove(bool dir);

	void Update(float deltaTime, float totalTime);
	Entity* getEntity() { return entity; }
private:
	bool oldAState;
	bool oldDState;
	bool oldWState;

	Mesh* mesh;
	Entity* entity;
	Level* level;
	int pos;
	float cooldown;
	int ammo;
};

