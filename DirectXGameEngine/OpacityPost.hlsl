
cbuffer externalData : register(b0)
{
	float4 playerPos; // in screen space
};

struct VertexToPixel
{
	float4 color		: COLOR;
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

Texture2D  diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

static float radius = 0.2;

float main(VertexToPixel input) : SV_TARGET
{
	float alpha = diffuseTexture.SampleLevel(basicSampler, input.uv, 0).r;
	float4 bet = playerPos - input.position;
	if (dot(bet, bet) < radius * radius) {
		return 0;
	}
	return 1;
}