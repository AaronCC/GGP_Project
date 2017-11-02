#include "Materials.h"

Materials::Materials(SimplePixelShader* _pix, SimpleVertexShader* _vert, ID3D11ShaderResourceView* _SRV,
	ID3D11SamplerState* _sample)
{
	pixShade = _pix;
	vertShade = _vert;
	SRV = _SRV;
	sampleState = _sample;
}


Materials::~Materials()
{
}

SimplePixelShader* Materials::GetPixShade()
{
	return pixShade;
}

SimpleVertexShader* Materials::GetVertShade()
{
	return vertShade;
}

ID3D11ShaderResourceView* Materials::GetSRV() 
{
	return SRV;
}

ID3D11SamplerState* Materials::GetSampleState()
{
	return sampleState;
}
