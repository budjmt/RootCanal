
cbuffer Data : register(b9)
{
	float pixelWidth;
	float pixelHeight;
	float distortAmount;
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



float4 main(VertexToPixel input) : SV_TARGET
{
	float distFromCenter = distance(float2(0.5f,0.5f), input.uv);
	
	float4 outColor = float4(0,0,0,1);
	
	float saturation = saturate(distFromCenter * pixelWidth * distortAmount);
	outColor.r = pixels.SampleLevel(trilinear, input.uv - float2(saturation,  saturation/2), 0).r;
	outColor.g = pixels.SampleLevel(trilinear, input.uv + float2(saturation,  saturation/2), 0).g;
	outColor.b = pixels.SampleLevel(trilinear, input.uv + float2(saturation, -saturation/2), 0).b;

	return outColor;
	return pixels.Sample(trilinear,input.uv);
}