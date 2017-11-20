
//cbuffer Data : register(b0)
//{
//	float pixelWidth;
//	float pixelHeight;
//	int blurAmount;
//}


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D Pixels		: register(t0);
SamplerState Sampler	: register(s0);
float SplitIntensity	: register(b0);


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	// Some standard average stuff
	float4 totalColor = float4(0,0,0,0);

	float4 ThisColor = Pixels.Sample(Sampler, input.uv); // get the color of this pixel
	
	//determine strength of aberration
	float splitFactor = 0.01 * SplitIntensity;

	// get the color of a specific neighboring pixel
	float4 redVal = Pixels.Sample(Sampler, float2(input.uv.x + splitFactor, input.uv.y + splitFactor));		//red -> top right
	float4 greenVal = Pixels.Sample(Sampler, float2(input.uv.x - splitFactor, input.uv.y - splitFactor));	//green -> bottom left
	float4 blueVal = Pixels.Sample(Sampler, float2(input.uv.x + splitFactor, input.uv.y - splitFactor));	//blue -> top left
	
	//overwrite this pixel's colors with the other smapled colors
	ThisColor.r = redVal.r;
	ThisColor.g = greenVal.g;
	ThisColor.b = blueVal.b;

	//add it to the total
	totalColor += ThisColor;
	
	//uint numSamples = 0;
	//
	//for (int y = -blurAmount; y <= blurAmount; y++)
	//{
	//	for (int x = -blurAmount; x <= blurAmount; x++)
	//	{
	//		// Figure out the uv coord of a neighboring pixel
	//		float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);
	//
	//		// Sample there!
	//		totalColor += Pixels.Sample(Sampler, uv);
	//
	//		numSamples++;
	//	}
	//}
	//
	//
	//totalColor += Pixels.Sample(Sampler, input.uv);// *float4(1.0, 0.5, 0.5, 1.0); //tints red

	// Average color
	return totalColor;// / numSamples;
}