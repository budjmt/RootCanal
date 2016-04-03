
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
    float3 normal       : NORMAL;
};

struct DirectionalLight
{
    float4 AmbientColor;
    float4 DiffuseColor;
    float3 Direction;
};

cbuffer Light : register( b0 )
{
    DirectionalLight light1;
    DirectionalLight light2;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
    input.normal = normalize(input.normal);

    float3 negatedDirection1 = -light1.Direction; // Negate light's direction
    float lightAmount1 = saturate( dot( input.normal, negatedDirection1 ) );
    float3 surfaceColor1 = lightAmount1 * light1.DiffuseColor + light1.AmbientColor;

    float3 negatedDirection2 = -light2.Direction; // Negate light's direction
    float lightAmount2 = saturate( dot( input.normal, negatedDirection2 ) );
    float3 surfaceColor2 = lightAmount2 * light2.DiffuseColor + light2.AmbientColor;

	return float4(surfaceColor1 + surfaceColor2, 1);
}