
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
	PointLight pointLight2;

	PointLight skyLight1;
	PointLight skyLight2;
	PointLight skyLight3;

	float3 CameraPosition;
};

Texture2D diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);

	//alpha clipping
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

	// POINT LIGHT ORIGIN
	float3 dirToPointLight2 = normalize(pointLight2.PL_Position - input.worldPos);
	float pointLight2Amount = saturate(dot(input.normal, dirToPointLight2));

	// POINT LIGHT SKY
	float3 dirToPointLightS1 = normalize(skyLight1.PL_Position - input.worldPos);
	float pointLightS1Amount = saturate(dot(input.normal, dirToPointLightS1));

	float3 dirToPointLightS2 = normalize(skyLight2.PL_Position - input.worldPos);
	float pointLightS2Amount = saturate(dot(input.normal, dirToPointLightS2));

	float3 dirToPointLightS3 = normalize(skyLight3.PL_Position - input.worldPos);
	float pointLightS3Amount = saturate(dot(input.normal, dirToPointLightS3));

	
	// point light shiny bit - currently unused
	//calc reflection vector of incoming light
	float3 refl = reflect(-dirToPointLight, input.normal);
	float3 refl2 = reflect(-dirToPointLight2, input.normal);
	float3 reflS1 = reflect(-dirToPointLightS1, input.normal);
	float3 reflS2 = reflect(-dirToPointLightS2, input.normal);
	float3 reflS3 = reflect(-dirToPointLightS3, input.normal);

	// Direction to the camera from the current pixel
	float3 dirToCamera = normalize(CameraPosition - input.worldPos);
	//Specular shiny spot
	float pointLightSpecular = pow(saturate(dot(refl, dirToCamera)), 64);
	float pointLight2Specular = pow(saturate(dot(refl2, dirToCamera)), 64);
	float pointLightS1Specular = pow(saturate(dot(reflS1, dirToCamera)), 64);
	float pointLightS2Specular = pow(saturate(dot(reflS2, dirToCamera)), 64);
	float pointLightS3Specular = pow(saturate(dot(reflS3, dirToCamera)), 64);

	//lights in the level
	lightsOut +=
		(pointLight1.PL_Color * pointLightAmount) +
		(pointLight2.PL_Color * pointLight2Amount) +
		(pointLightSpecular)+
		(pointLight2Specular);

	//lights for the skyplane
	lightsOut +=
		(skyLight1.PL_Color * pointLightS1Amount) +
		(skyLight2.PL_Color * pointLightS2Amount) +
		(skyLight3.PL_Color * pointLightS3Amount) +
		(pointLightS1Specular) +
		(pointLightS2Specular) +
		(pointLightS3Specular);

	//return float4(surfaceColor.yyy, 1.0f);
	return surfaceColor * lightsOut;
	

	//return float4(input.normal, 1);
	//return light.DiffuseColor;
	//return float4(1,0,0,1);
}

