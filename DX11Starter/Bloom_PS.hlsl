// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D Pixels		: register(t0);
Texture2D Pixels2		: register(t1);
SamplerState Sampler	: register(s0);
float Purpose			: register(f0);
float DesatVal			: register(f1);
int BlurAmount			: register(i0);
float PixelWidth		: register(f2);
float PixelHeight		: register(f3);


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//get this pixel color
	float4 ThisColor = Pixels.Sample(Sampler, input.uv);
	
	//these are the values for greyscale
	float4 greyscale = (0.3, 0.59, 0.11);

	//container for blurring
	float4 totalColor = float4(0, 0, 0, 0);

	//the number of samples for averaging color in blur
	uint numSamples = 0;

	//ps performs multiple tasks
	switch (Purpose) {
		
		//task 1 is to get the "bright" pixels
		case 0:
			float4 desaturated;
			desaturated.r = saturate((ThisColor.r - DesatVal) / (1 - DesatVal));
			desaturated.g = saturate((ThisColor.g - DesatVal) / (1 - DesatVal));
			desaturated.b = saturate((ThisColor.b - DesatVal) / (1 - DesatVal));
			return desaturated;
			break;

		//task 2 is to blur horizontally
		case 1:
			totalColor = float4(0, 0, 0, 0);
			numSamples = 0;
			for (int x = -BlurAmount + 1; x < BlurAmount; x++) //this new for loop skips the two extremes, because they will be always be 0%
			{
				//starting at left, moving right, sample pixels
				float2 uv = input.uv + float2(x * PixelWidth, 0); // get the new x and same y
				totalColor += Pixels.Sample(Sampler, uv) * (1 - abs(x / BlurAmount)); // make the color fall off as it goes away from the center

				//increment number of samples
				numSamples ++;
			}
			return totalColor / numSamples;
			break;

		//task 3 is to blur vertically
		case 2:
			totalColor = float4(0, 0, 0, 0);
			numSamples = 0;
			for (int y = -BlurAmount + 1; y < BlurAmount; y++)
			{
				//starting at left, moving right, sample pixels
				float2 uv = input.uv + float2(0, y * PixelHeight);
				totalColor += Pixels.Sample(Sampler, uv) * (1 - abs(y / BlurAmount));

				//increment number of samples
				numSamples ++;
			}
			return totalColor / numSamples;
			break;

		//task 4 is to recombine the final color and the bloom effect
		case 3:
			totalColor = float4(0, 0, 0, 0);
			totalColor = Pixels2.Sample(Sampler, input.uv) + Pixels.Sample(Sampler, input.uv);
			return totalColor;
			break;
	}

	//return black - this should never be a thing
	return 0;
}