
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{ 
	float4 position		: POSITION;
	float4 color		: COLOR;
};

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;
	matrix worldViewProj = mul(view, projection);

	output.position = mul(input.position, worldViewProj);
	output.color = float4(input.color.xyz, 1);

	return output;
}