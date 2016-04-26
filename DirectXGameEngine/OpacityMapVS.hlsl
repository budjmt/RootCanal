

cbuffer externalData : register(b0)
{
	matrix world;
	matrix inv_trans_world;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{
	float4 color		: COLOR;        // RGBA color
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 color		: COLOR;        // RGBA color
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

VertexToPixel main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixel output;

	matrix worldViewProj = mul(mul(world, view), projection);

	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	output.uv = input.uv;
	//output.normal = normalize(mul(float4(input.normal,0.f), inv_trans_world).xyz);
	output.normal = normalize(mul(input.normal, (float3x3)world));

	output.color = input.color;

	return output;
}