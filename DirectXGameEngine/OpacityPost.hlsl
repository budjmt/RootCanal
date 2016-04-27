
cbuffer externalData : register(b0)
{
	float4 playerPos; // in screen space
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

Texture2D  diffuseTexture : register(t0);
SamplerState basicSampler : register(s0);

static int radius = 50;

float main(VertexToPixel input) : SV_TARGET
{
	float alpha = diffuseTexture.SampleLevel(basicSampler, input.uv, 0);
	float4 bet = playerPos - input.position;
	if (dot(bet, bet) < radius * radius) {
		return 0;
	}
	return alpha;
}