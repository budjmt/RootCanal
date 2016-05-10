
RWTexture2D<float4> outTex : register(u0);

cbuffer data : register (b0) {
	int width;
	float2 relativePos;
}

[numthreads(8,8,1)]
void main(uint3 id : SV_DispatchThreadID) {
	/*
	if(id.x < 100 && id.y < 100) outTex[id.xy] = float4(0, 0, 0, 1);
	else outTex[id.xy] = float4(1, 0.5, 0.25, 1);
	*/


	//outTex[id.xy] = distance(relativePos/width, id.xy/width);
	outTex[id.xy] = float4(id.x, id.y, 0, 1) / width;
}