
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
};

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 color = float4(0., 0.5, 0.7, 0.2);
	return color;
}