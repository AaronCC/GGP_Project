#include "Level.h"

using namespace DirectX;

Level::Level(Materials* mat)
{
	material = mat;
}

Level::~Level()
{

}

void Level::genLevel(ID3D11Device*	device,
	int* inds, // int array (0..n) where n = 6 * number of lanes
	Vertex* verts, // Vertex array (null) where length = 2 * number of lanes
	const int LANE_COUNT, const float LENGTH, const int MAX_VARIANCE, const float DEPTH)
{
	float angle = 0.f;

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	// Calc near face
	for (int i = 0; i < LANE_COUNT; i++)
	{
		// Calc length from origin
		float length = LENGTH + (static_cast <float> (std::rand() / static_cast <float> (MAX_VARIANCE)));
		// calc vertex position
		positions.push_back({ length * std::cos(angle), length * std::sin(angle), 0 });
		// incriment angle value
		angle += (2.f * XM_PI) / LANE_COUNT;
		// calc normal
		XMStoreFloat3(&normals[i], XMVector3Normalize(XMVECTOR{ -1 * positions[i].x, -1 * positions[i].y, 0.f }));
	}
	// Calc far face
	int i = 0;
	for each(XMFLOAT3 pos in positions)
	{
		positions.push_back(XMFLOAT3(pos.x, pos.y, DEPTH));
		XMStoreFloat3(&normals[i], XMVector3Normalize(XMVECTOR{ -1 * pos.x, -1 * pos.y, 0.f }));
		i++;
	}

	// Calc inds
	for (int i = 0; i < (LANE_COUNT - 1) * 6; i += 6)
	{
		int x = i / 6;
		inds[i] = x;
		inds[i + 1] = x + 1;
		inds[1 + 2] = x + 1 + LANE_COUNT;
		inds[i + 3] = x + 1 + LANE_COUNT;
		inds[i + 4] = x + LANE_COUNT;
		inds[i + 5] = x;
	}

	// Last face
	int f = LANE_COUNT * 6 - 1;
	inds[f - 5] = LANE_COUNT - 1;
	inds[f - 4] = 0;
	inds[f - 3] = LANE_COUNT;
	inds[f - 2] = LANE_COUNT;
	inds[f - 1] = LANE_COUNT * 2 - 1;
	inds[f] = LANE_COUNT - 1;

	// Calc Verts
	for (int i = 0; i < LANE_COUNT * 6; i++)
	{
		verts[i] = Vertex{
			positions[i],
			normals[i],
			XMFLOAT2{0,0}
		};
	}

	// Calc lanes
	for (int i = 0; i < LANE_COUNT; i++)
	{
		XMFLOAT2 mid;
		if (i == LANE_COUNT - 1)
			mid = { (positions[i].x + positions[i + 1].x) / 2.f,(positions[i].y + positions[i + 1].y) / 2.f };
		else
			mid = { (positions[i].x + positions[0].x) / 2.f,(positions[i].y + positions[0].y) / 2.f };

		lanes.push_back(&Lane(mid, DEPTH));
	}

	// level mesh
	level = &Entity(&Mesh(verts, LANE_COUNT * 2, inds, LANE_COUNT * 6, device), material);
}
