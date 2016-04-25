
cbuffer Data : register(b0)
{
	float pixelWidth;
	float pixelHeight;
	int blurAmount;
}


// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

// Textures and such
Texture2D pixels		: register(t0);
SamplerState trilinear	: register(s0);
float3 camPos			: register(c0);


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//return pixels.Sample(trilinear, input.uv);
	//return float4(1,0,0,1);
	float4 totalColor = float4(0,0,0,0);
	uint sampleCount = 0;

	totalColor += pixels.Sample(trilinear, input.uv);
	sampleCount++;

	for (int i = 1; i <= blurAmount; i++) {
		float2 uvMod = float2(0, i * pixelHeight);

		totalColor += pixels.Sample(trilinear, input.uv + uvMod);
		totalColor += pixels.Sample(trilinear, input.uv - uvMod);

		sampleCount += 2;

		uvMod = float2(i * pixelHeight, 0);

		totalColor += pixels.Sample(trilinear, input.uv + uvMod);
		totalColor += pixels.Sample(trilinear, input.uv - uvMod);

		sampleCount += 2;
	}

	return (totalColor / sampleCount);
}