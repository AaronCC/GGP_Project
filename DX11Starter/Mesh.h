#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include "Vertex.h"
#include <iostream>
#include <fstream>
#include <vector>


class Mesh
{
public:
	Mesh(Vertex* _vertArray, int _vertNum, int* _indArray, int _indNum, ID3D11Device* _ID3D11_Obj);
	
	Mesh(char* _filename, ID3D11Device* _ID3D11_Obj);
	
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

private:
	ID3D11Buffer* vertexBuffer; //vertex buffer
	ID3D11Buffer* indexBuffer; //index buffer
	int numIndices; //number of indicies in index buffer (dexBuf)
};

