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

	void genLevel(ID3D11Device*	device, int* inds, Vertex * verts, const int LANE_COUNT, const float LENGTH, const int MAX_VARIANCE, const float DEPTH);
	
private:
	std::vector<Lane*> lanes;
	Entity* level;
	Materials* material;
};

