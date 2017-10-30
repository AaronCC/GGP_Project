#include "Level.h"

using namespace DirectX;

Level::Level(Materials* mat)
{
	material = mat;
}

Level::~Level()
{
	levelMesh->~Mesh();
	levelEntity->~Entity();
	//lanes.swap(0); //delete the lanes vector
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
		float length = LENGTH + (static_cast <float> (std::rand() % MAX_VARIANCE));
		// calc vertex position
		positions.push_back({ length * std::cos(angle), length * std::sin(angle), 0 });
		// incriment angle value
		angle += (XM_2PI) / LANE_COUNT;
		// calc normal
		XMFLOAT3 normCatch;
		XMStoreFloat3(&normCatch, XMVector3Normalize(XMVECTOR{ (-1 * positions[i].x), (-1 * positions[i].y), (0.f) }));
		normals.push_back(normCatch);
	}
	// Calc far face
	for(int n = 0; n < LANE_COUNT; n++)
	{
		positions.push_back(XMFLOAT3{ positions[n].x, positions[n].y, DEPTH });
	}

	for (int p = 0; p < LANE_COUNT; p++)
	{
		normals.push_back(normals[p]);
	}

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
	for (int i = 0; i < LANE_COUNT * 2; i++)
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

		lanes.push_back(Lane(mid, DEPTH));
	}

	// level mesh
	levelMesh = new Mesh(verts, LANE_COUNT * 2, inds, LANE_COUNT * 6, device);
	levelEntity = new Entity(levelMesh, material);
}
