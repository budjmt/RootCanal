
struct DirectionalLight {
	float4 ambientColor;
	float4 diffuseColor;
	float3 direction;
};

cbuffer externalData : register(b0) {
	DirectionalLight light1, light2;
	float opacity;
};

Texture2D diffuseTexture0 : register(t0);//actual texture
Texture2D diffuseTexture1 : register(t1);//opacity map
Texture2D diffuseTexture2 : register(t2);//opacity map
SamplerState basicSampler : register(s0);

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
	float4 color		: COLOR;
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;

};

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

	float tunnelAlpha = diffuseTexture2.Sample(basicSampler,input.uv).r;

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	float3 light1Dir = normalize(-light1.direction);
	float lamb1 = saturate(dot(input.normal, light1Dir));
	//lamb1 = ceil(lamb1 * 3) / 3;

	float3 light2Dir = normalize(-light2.direction);
	float lamb2 = saturate(dot(input.normal, light2Dir));
	//lamb2 = ceil(lamb2 * 3) / 3;

	float4 surfaceColor = diffuseTexture0.Sample(basicSampler, input.uv);
	//surfaceColor = float4(0, 1, 0, 0.5f);
	surfaceColor.a = diffuseTexture1.SampleLevel(basicSampler, input.uv,0).r;//it's one channel
	/*surfaceColor *=
		(light1.diffuseColor * lamb1 + light1.ambientColor
			+ light2.diffuseColor * lamb2 + light2.ambientColor);*/
	surfaceColor.w = tunnelAlpha;
	return float4(surfaceColor.xyz,surfaceColor.w * pow(opacity,2));
}