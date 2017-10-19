
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	//float4 color		: COLOR;        // RGBA color
	float3 normal       : NORMAL;       // Normals
	float2 uv			: TEXCOORD;		// UV Coords
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;

};

cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float4 lightsOut = float4(0,0,0,0);

	input.normal = normalize(input.normal);
	float3 dirNorm = normalize(-light.Direction);
	float dotSatLight = saturate(dot(input.normal, dirNorm));

	lightsOut += light.DiffuseColor * dotSatLight + light.AmbientColor;

	float3 dirNorm2 = normalize(-light2.Direction);
	float dotSatLight2 = saturate(dot(input.normal, dirNorm2));

	lightsOut += light2.DiffuseColor * dotSatLight2 + light2.AmbientColor;

	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	return lightsOut * surfaceColor;

	//return float4(input.normal, 1);
	//return light.DiffuseColor;
	//return float4(1,0,0,1);
}

