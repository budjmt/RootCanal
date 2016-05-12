
cbuffer Data : register(b0)
{
	float pixelWidth;
	float pixelHeight;
	int blurAmount;
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
Texture2D bright		: register(t1);
Texture2D test		    : register(t6);
SamplerState trilinear	: register(s0);


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	//return float4(1,1,1,1);
	//return pixels.Sample(trilinear, input.uv);
	//return test.Sample(trilinear, input.uv);

	//return float4(1,1,0.5f,1);
	float4 totalColorX = test.Sample(trilinear, input.uv);
	float4 totalColorY = test.Sample(trilinear, input.uv);
	uint sampleCount = blurAmount * 2 + 1;


	/*for (int i = 1; i <= blurAmount; i++) {
		float2 uvMod = float2(i * pixelHeight, 0);
		totalColorX += test.SampleLevel(trilinear, input.uv + uvMod, 0);
		totalColorX += test.SampleLevel(trilinear, input.uv - uvMod, 0);
	}

	for (int i = 1; i <= blurAmount; i++) {
		float2 uvMod = float2(0, i * pixelHeight);
		totalColorY += test.SampleLevel(trilinear, input.uv + uvMod, 0);
		totalColorY += test.SampleLevel(trilinear, input.uv - uvMod, 0);
	}

	totalColorY /= sampleCount;
	totalColorX /= sampleCount;

	float totalColor = (totalColorY + totalColorX) / 2;*/

	float4 totalColor = 0;

	sampleCount = 1;



	//multi pass box blur
	//faster than box, not as fast as gaussian
	for (int z = 1; z < 5; z++) {
		//the shameful box blur
		for (int y = 1; y <= blurAmount*z; y+=z)
		{
			for (int x = 1; x <= blurAmount*z; x+=z)
			{
				float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);
				uv.x = saturate(uv.x);
				uv.y = saturate(uv.y);
				totalColor += test.Sample(trilinear, uv);

				sampleCount++;

				uv = input.uv - float2(x * pixelWidth, y * pixelHeight);
				uv.x = saturate(uv.x);
				uv.y = saturate(uv.y);
				totalColor += test.Sample(trilinear, uv);

				sampleCount++;

				uv = input.uv + float2(-x * pixelWidth, y * pixelHeight);
				uv.x = saturate(uv.x);
				uv.y = saturate(uv.y);
				totalColor += test.Sample(trilinear, uv);

				sampleCount++;

				uv = input.uv + float2(x * pixelWidth, -y * pixelHeight);
				uv.x = saturate(uv.x);
				uv.y = saturate(uv.y);
				totalColor += test.Sample(trilinear, uv);

				sampleCount++;
			}
		}
	}
	////the shameful box blur
	//for (int y = 1; y <= blurAmount; y++)
	//{
	//	for (int x = 1; x <= blurAmount; x++)
	//	{
	//		float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;

	//		uv = input.uv - float2(x * pixelWidth, y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;

	//		uv = input.uv + float2(-x * pixelWidth, y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;

	//		uv = input.uv + float2(x * pixelWidth, -y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;
	//	}
	//}

	////the shameful box blur
	//for (int y = 1; y <= blurAmount*2; y+=2)
	//{
	//	for (int x = 1; x <= blurAmount*2; x+=2)
	//	{
	//		float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;

	//		uv = input.uv - float2(x * pixelWidth, y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;
	//	}
	//}

	////the shameful box blur
	//for (int y = 1; y <= blurAmount * 3; y += 3)
	//{
	//	for (int x = 1; x <= blurAmount * 3; x += 3)
	//	{
	//		float2 uv = input.uv + float2(x * pixelWidth, y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;

	//		uv = input.uv - float2(x * pixelWidth, y * pixelHeight);
	//		uv.x = saturate(uv.x);
	//		uv.y = saturate(uv.y);
	//		totalColor += test.Sample(trilinear, uv);

	//		sampleCount++;
	//	}
	//}

	totalColor /= sampleCount;

	return totalColor + pixels.SampleLevel(trilinear, input.uv, 0);
}