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
	return (inColor.r + inColor.g + inColor.b) /3;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 blurColor = blur.Sample(trilinear, input.uv);
	bool mask = blurColor.g > 0.5f;

	float4 totalColorX = pixels.Sample(trilinear, input.uv);
	float4 totalColorY = pixels.Sample(trilinear, input.uv);
	uint sampleCount = 1;
	
	if (mask) {
		for (int v = 1; v <= blurAmount; v++) {
			float2 uvMod = float2(0, v * pixelHeight);

			totalColorX += pixels.SampleLevel(trilinear, input.uv + uvMod, 0);
			totalColorX += pixels.SampleLevel(trilinear, input.uv - uvMod, 0);

			sampleCount += 2;
		}
		totalColorX /= sampleCount;

		sampleCount = 1;
		for (int u = 1; u <= blurAmount; u++) {
			float2 uvMod = float2(u * pixelHeight, 0);

			totalColorY += pixels.SampleLevel(trilinear, input.uv + uvMod, 0);
			totalColorY += pixels.SampleLevel(trilinear, input.uv - uvMod, 0);

			sampleCount += 2;
		}
		totalColorY /= sampleCount;
	}
	
	return ((totalColorX + totalColorY) / 2);
}