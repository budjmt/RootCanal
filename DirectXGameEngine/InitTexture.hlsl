
RWTexture2D<float4> outTex : register(u0);


[numthreads(8, 8, 1)]
void main(uint3 id : SV_DispatchThreadID) {
	outTex[id.xy] = float4(1, 1, 1, 1);
}