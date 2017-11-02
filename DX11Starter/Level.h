#pragma once

#include "Lane.h"
#include "Entity.h"
#include "Materials.h"
#include <vector>
#include <DirectXMath.h>
#include <stdlib.h>

class Level
{
public:
	Level(Materials* mat);
	~Level();

	void genLevel(ID3D11Device*	device, int* inds, Vertex * verts, const int LANE_COUNT, const float LENGTH, const int MAX_VARIANCE, const float DEPTH, Materials* enemyMat, Materials* projMat);
	Entity* getEntity() { return levelEntity; }
	Mesh* getMesh() { return levelMesh; }
	int getLaneCount() { return lanes.size(); }
	XMFLOAT3 getLanePos(int index) { return lanes[index]->getPos(); }
	std::vector<Lane*>* getLanes() { return &lanes; }
	Lane* getLane(int index) { return lanes[index]; }
	
	void Update(float deltaTime, float totalTime);

private:
	std::vector<Lane*> lanes;
	Entity* levelEntity;
	Mesh* levelMesh;
	Materials* material;
};

