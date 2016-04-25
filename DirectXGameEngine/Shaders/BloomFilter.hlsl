
cbuffer Data : register(b0)
{
	float minBrightness;
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


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 color = pixels.Sample(trilinear, input.uv);
	//return color;

	if (color.x >= minBrightness || color.y >= minBrightness || color.z >= minBrightness) {
		return color;
	}
	
	return float4(0, 0, 0, 1);
}