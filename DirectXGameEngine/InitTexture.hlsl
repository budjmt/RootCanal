
RWTexture2D<float> outTex : register(u0);


[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID) {
	outTex[id.xy] = 1;
}