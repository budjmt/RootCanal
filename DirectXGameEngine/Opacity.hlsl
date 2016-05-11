
RWTexture2D<float> outTex	: register(u0);
RWTexture2D<float> readTex	: register(u5);

cbuffer data : register (b3) {
	uint width;
	float2 relativePos;
}

[numthreads(8,8,1)]
void main(uint3 id : SV_DispatchThreadID) {
	/*
	if(id.x < 100 && id.y < 100) outTex[id.xy] = float4(0, 0, 0, 1);
	else outTex[id.xy] = float4(1, 0.5, 0.25, 1);
	*/



	float2 uvPos = float2(id.x, id.y);

	if (distance(relativePos.yx * width, uvPos) < 20) {
		outTex[id.xy] = 0;// float4(0, 0, 0, 0);
	}
	else {
		outTex[id.xy] = 1 * readTex[id.xy];// float4(1, 1, 1, 1);// readTex[id.xy];//float4(1, 1, 1, 1);// *readTex[id.xy].w;
		//readTex[id.xy] = float4(0, 1, 1, 1);
	}


	/*if (distance(relativePos.yx * width, uvPos) < 30) outTex[id.xy] = float4(0, 0, 0, 1);
	else outTex[id.xy] = float4(1, 0.5, 0.25, 1);*/

	//outTex[id.xy] = distance(float2(0,0), uvPos);
	//outTex[id.xy] = float4(id.x, id.y, 0, 1) / width;
}