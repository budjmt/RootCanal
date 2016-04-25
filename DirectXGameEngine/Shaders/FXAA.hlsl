// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

cbuffer Data : register(c0) {
	float thresholdMin;
	float thresholdMax;
	float pixelWidth;
	float pixelHeight;
	int blurAmount;
}

// Textures and such
Texture2D pixels		: register(t0);
Texture2D blur			: register(t8);
SamplerState trilinear	: register(s0);

float pixelIntensity(float4 inColor) {
	//simple average of rgb, may change later
	return (inColor.x + inColor.y + inColor.z) /3;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 blurColor = blur.Sample(trilinear, input.uv);

	bool mask = blurColor.y > 0.5f;

	float4 totalColor = float4(0, 0, 0, 0);
	uint sampleCount = 0;
	
	totalColor += pixels.Sample(trilinear, input.uv);
	sampleCount++;
	
	for (int i = 1; i <= blurAmount; i++) {
		if (mask) {
			float2 uvMod = float2(0, i * pixelHeight);

			totalColor += pixels.Sample(trilinear, input.uv + uvMod);
			totalColor += pixels.Sample(trilinear, input.uv - uvMod);

			sampleCount += 2;

			uvMod = float2(i * pixelHeight, 0);

			totalColor += pixels.Sample(trilinear, input.uv + uvMod);
			totalColor += pixels.Sample(trilinear, input.uv - uvMod);

			sampleCount += 2;
		}
	}
	
	return (totalColor / sampleCount);
}