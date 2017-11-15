
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
	float3 worldPos		: POSITION;		// world position of pixel
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

struct PointLight
{
	float4 PL_Color;
	float3 PL_Position;
};

cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;

	PointLight pointLight1;

	float3 CameraPosition;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	if (surfaceColor.a < 0.2f)
		discard;

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float4 lightsOut = float4(0,0,0,0); // output light values

	input.normal = normalize(input.normal);
	float3 dirNorm = normalize(-light.Direction);
	float dotSatLight = saturate(dot(input.normal, dirNorm));

	//lightsOut += light.DiffuseColor * dotSatLight + light.AmbientColor;

	float3 dirNorm2 = normalize(-light2.Direction);
	float dotSatLight2 = saturate(dot(input.normal, dirNorm2));

	//lightsOut += light2.DiffuseColor * dotSatLight2 + light2.AmbientColor;

	// POINT LIGHT
	float3 dirToPointLight = normalize(pointLight1.PL_Position - input.worldPos);
	float pointLightAmount = saturate(dot(input.normal, dirToPointLight));

	/*
	// point light shiny bit - currently unused
	//calc reflection vector of incoming light
	float3 refl = reflect(-dirToPointLight, input.normal);
	// Direction to the camera from the current pixel
	float3 dirToCamera = normalize(CameraPosition - input.worldPos);
	//Specular shiny spot
	float pointLightSpecular = pow(saturate(dot(refl, dirToCamera)), 64);
	*/

	lightsOut += pointLight1.PL_Color * pointLightAmount;

	//return float4(surfaceColor.yyy, 1.0f);
	return surfaceColor * lightsOut;
	

	//return float4(input.normal, 1);
	//return light.DiffuseColor;
	//return float4(1,0,0,1);
}

