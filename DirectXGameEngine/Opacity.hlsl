
RWTexture2D<float> outTex	: register(u0);

cbuffer data : register (b3) {
	uint width;
	float2 relativePos;
}

[numthreads(8,8,1)]
void main(uint3 id : SV_DispatchThreadID) {

	float2 uvPos = float2(id.x, id.y);

	if (distance(relativePos.yx * width, uvPos) < 20) {
		outTex[id.xy] = 0;
	}
	else outTex[id.xy] = 1 * outTex[id.xy];


}