#pragma once
#include "Level.h"
#include "Mesh.h"
#include <DirectXMath.h>
#include "Materials.h"
#include "DXCore.h"

class Player
{
public:
	Player(Level * level, Materials* material, Materials* outlineMat, ID3D11Device* device);
	~Player();

	void WheelMove(bool dir);

	void Update(float deltaTime, float totalTime);
	Entity* getEntity() { return entity; }
	Entity* getOutlineEntity() { return outlineEntity; }
	void setLevel(Level* newLevel) { level = newLevel; }
	void setPos(int inPos) { pos = inPos; }

private:
	bool oldAState;
	bool oldDState;
	bool oldWState;

	Mesh* mesh;
	Entity* entity;
	Entity* outlineEntity;
	Level* level;
	int pos;
	float cooldown;
	int ammo;
};

