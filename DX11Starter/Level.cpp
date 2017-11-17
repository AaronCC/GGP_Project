#include "Level.h"
#include "time.h"

using namespace DirectX;

Level::Level(Materials* mat)
{
	material = mat;
}

Level::~Level()
{
	delete levelMesh;
	delete levelEntity;
	for each(Lane* lane in lanes)
		lane->~Lane();
}

void Level::genLevel(ID3D11Device*	device,
	int* inds, // int array (0..n) where n = 6 * number of lanes
	Vertex* verts, // Vertex array (null) where length = 2 * number of lanes
	const int LANE_COUNT, const float LENGTH, const int MAX_VARIANCE, const float DEPTH, 
	Materials* enemyMat, Materials* projMat)
{
	srand(time(NULL));
	float angle = 0.f;

	//clear out the old lanes
	lanes.clear();

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> uvs;
	bool uv = false; // 0 or 1
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
		uvs.push_back(XMFLOAT2{ (float)uv,0.f });
		uv = !uv;
	}
	// Calc far face
	for(int n = 0; n < LANE_COUNT; n++)
	{
		positions.push_back(XMFLOAT3{ positions[n].x, positions[n].y, DEPTH });
	}

	//calc normals and uvs for back face
	for (int p = 0; p < LANE_COUNT; p++)
	{
		normals.push_back(normals[p]);
		uvs.push_back(XMFLOAT2{ uvs[p].x,1.f });
	}

	//do math
	for (int i = 0; i < (LANE_COUNT - 1) * 6; i += 6)
	{
		int x = i / 6;
		inds[i]		= x + 1;
		inds[i + 1] = x;
		inds[i + 2] = x + 1 + LANE_COUNT;
		inds[i + 3] = x + 1 + LANE_COUNT;
		inds[i + 4] = x;
		inds[i + 5] = x + LANE_COUNT;
	}

	// do last face math
	int f = LANE_COUNT * 6 - 1;		//f = end of index array
	inds[f - 5] = 0;				//first index of last quad
	inds[f - 4] = LANE_COUNT - 1;	//second
	inds[f - 3] = LANE_COUNT;		//third
	inds[f - 2] = LANE_COUNT;		//fourth
	inds[f - 1] = LANE_COUNT - 1;	//fifth
	inds[f] = LANE_COUNT * 2 - 1;	//sixth

	// Calc Verts
	for (int i = 0; i < LANE_COUNT * 2; i++)
	{
		verts[i] = Vertex{
			positions[i],
			normals[i],
			uvs[i]
		};
	}

	// Calc lanes
	for (int i = 0; i < LANE_COUNT; i++)
	{
		XMFLOAT2 mid;
		if (i == LANE_COUNT - 1)
			mid = { (positions[i].x + positions[0].x) / 2.f,(positions[i].y + positions[0].y) / 2.f };
		else
			mid = { (positions[i].x + positions[i + 1].x) / 2.f,(positions[i].y + positions[i + 1].y) / 2.f };

		lanes.push_back(new Lane(mid, DEPTH, enemyMat, projMat, device));
	}

	// level mesh
	levelMesh = new Mesh(verts, LANE_COUNT * 2, inds, LANE_COUNT * 6, device);
	levelEntity = new Entity(levelMesh, material);
}

void Level::Update(float deltaTime, float totalTime)
{
	//update all the lanes
	for each(Lane* lane in lanes)
	{
		lane->Update(deltaTime, totalTime);
	}
}
