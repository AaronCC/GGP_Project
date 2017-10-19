#pragma once
#include "SimpleShader.h"

class Materials
{
public:
	Materials(SimplePixelShader* _pix, SimpleVertexShader* _vert, ID3D11ShaderResourceView* _SRV, 
		ID3D11SamplerState* _sample
	);
	~Materials();

	SimplePixelShader* pixShade; //pixel shader pointer
	SimpleVertexShader* vertShade; //vertex shader pointer

	ID3D11ShaderResourceView* SRV;
	ID3D11SamplerState* sampleState;
	

	SimplePixelShader* GetPixShade();
	SimpleVertexShader* GetVertShade();
	ID3D11ShaderResourceView* GetSRV();
	ID3D11SamplerState* GetSampleState();
};

