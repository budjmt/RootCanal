
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

struct VertexInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
};

struct InstanceInput 
{
	matrix world		: WORLD_MAT;
	float4 color		: COLOR;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
	float3 normal		: NORMAL;
};

VertexToPixel main( VertexInput v, InstanceInput i )
{	
	matrix worldViewProj = mul(mul(i.world, view), projection);

	VertexToPixel output;
	output.position = mul(float4(v.position, 1.0f), worldViewProj);
	output.normal = normalize(mul(v.normal, (float3x3)i.world));

	return output;
}