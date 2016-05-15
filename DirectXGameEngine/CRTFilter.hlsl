
cbuffer Data : register(c0) {
	uint width;
	uint height;
	float time;
}

struct VertexToPixel {
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD0;
};

Texture2D	 pixels		: register(t0);
SamplerState trilinear	: register(s0);

// Emulated input resolution.
/*#if 0
// Fix resolution to set amount.
float2 res = float2(320.0 / 1.0, 160.0 / 1.0);
#else
// Optimize for resize.
float2 res = resolution.xy / 6.0
#endif*/

// Hardness of scanline.
//  -8.0 = soft
// -16.0 = medium
//float hardScan = -8.0;

// Hardness of pixels in scanline.
// -2.0 = soft
// -4.0 = hard
float hardPix = -3.0;

// Amount of shadow mask.
//float maskDark = 0.5;
//float maskLight = 1.5;

//------------------------------------------------------------------------

// sRGB to Linear.
// Assuing using sRGB typed textures this should not be needed.
float ToLinear1(float c) { return (c <= 0.04045) ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4); }
float3 ToLinear(float3 c) { return float3(ToLinear1(c.r), ToLinear1(c.g), ToLinear1(c.b)); }

// Linear to sRGB.
// Assuing using sRGB typed textures this should not be needed.
float ToSrgb1(float c) { return (c < 0.0031308 ? c * 12.92 : 1.055 * pow(c, 0.41666) - 0.055); }
float3 ToSrgb(float3 c) { return float3(ToSrgb1(c.r), ToSrgb1(c.g), ToSrgb1(c.b)); }

// Nearest emulated sample given floating point position and texel offset.
// Also zero's off screen.
float3 Fetch(float2 pos, float2 off, float2 res) {
	pos = floor(pos * res + off) / res;
	if (max(abs(pos.x - 0.5), abs(pos.y - 0.5)) > 0.5)
		return float3(0.0, 0.0, 0.0);
	else//shut up compiler
		return ToLinear(pixels.SampleLevel(trilinear, pos.xy, 0).rgb);
}

// Distance in emulated pixels to nearest texel.
float2 Dist(float2 pos, float2 res) { 
	pos = pos*res; 
	return -((pos - floor(pos)) - float2(0.5,0.5)); 
}

// 1D Gaussian.
float Gaus(float pos, float scale) { 
	return exp2(scale*pos*pos); 
}

// 3-tap Gaussian filter along horz line.
float3 Horz3(float2 pos, float off, float2 res) {
	float3 b = Fetch(pos, float2(-1.0, off), res);
	float3 c = Fetch(pos, float2( 0.0, off), res);
	float3 d = Fetch(pos, float2( 1.0, off), res);
	float dst = Dist(pos, res).x;
	// Convert distance to weight.
	float scale = hardPix;
	float wb = Gaus(dst - 1.0, scale);
	float wc = Gaus(dst + 0.0, scale);
	float wd = Gaus(dst + 1.0, scale);
	// Return filtered sample.
	return (b*wb + c*wc + d*wd) / (wb + wc + wd);
}

// 5-tap Gaussian filter along horz line.
float3 Horz5(float2 pos, float off, float2 res) {
	float3 a = Fetch(pos, float2(-2.0, off), res);
	float3 b = Fetch(pos, float2(-1.0, off), res);
	float3 c = Fetch(pos, float2( 0.0, off), res);
	float3 d = Fetch(pos, float2( 1.0, off), res);
	float3 e = Fetch(pos, float2( 2.0, off), res);
	float dst = Dist(pos, res).x;
	// Convert distance to weight.
	float scale = hardPix;
	float wa = Gaus(dst - 2.0, scale);
	float wb = Gaus(dst - 1.0, scale);
	float wc = Gaus(dst + 0.0, scale);
	float wd = Gaus(dst + 1.0, scale);
	float we = Gaus(dst + 2.0, scale);
	// Return filtered sample.
	return (a*wa + b*wb + c*wc + d*wd + e*we) / (wa + wb + wc + wd + we);
}

// Return scanline weight.
float Scan(float2 pos, float off, float2 res, float hardScan) {
	float dst = Dist(pos, res).y;
	return Gaus(dst + off, hardScan);
}

// Allow nearest three lines to effect pixel.
float3 Tri(float2 pos, float2 res, float hardScan) {
	float3 a = Horz3(pos, -1.0, res);
	float3 b = Horz5(pos,  0.0, res);
	float3 c = Horz3(pos,  1.0, res);
	float wa = Scan (pos, -1.0, res, hardScan);
	float wb = Scan (pos,  0.0, res, hardScan);
	float wc = Scan (pos,  1.0, res, hardScan);
	return a*wa + b*wb + c*wc;
}

// Display warp.
// 0.0 = none
// 1.0/8.0 = extreme
float2 warp = float2(1.0 / 32.0, 1.0 / 24.0);
// Distortion of scanlines, and end of screen alpha.
float2 Warp(float2 pos) {
	pos = (pos * 2.0) - 1.0;
	pos *= float2(1.05 + (pos.y * pos.y) * warp.x, 1.05 + (pos.x * pos.x) * warp.y);
	return (pos * 0.5) + 0.5;
}

// Shadow mask.
float3 Mask(float2 pos, float maskDark, float maskLight) {
	pos.x += pos.y * 2.0;//change the const value to change the pixels a bit
	float3 mask = float3(maskDark, maskDark, maskDark);
	pos.x = frac(pos.x / 6.0);
	if (pos.x < 0.333)
		mask.r = maskLight;
	else if (pos.x < 0.666)
		mask.g = maskLight;
	else 
		mask.b = maskLight;
	return mask;
}

float3 ScanMask(float posY, float maskDark, float maskLight) {
	//return float3(1, 1, 1);
	//posY *= 3;
	float p = 0.75 * (fmod(time * 0.3, 2.0) - 1) + 0.5;
	float r = 0.035;
	float d = abs(posY - p) - r;
	if (d <= 0)
		return float3(maskLight, maskLight, maskLight) * (1.5 - clamp(5 * exp2(d), 0, 1.5));
	else
		return float3(maskDark, maskDark, maskDark);
}

// Entry.
//credit to https://www.shadertoy.com/view/XsjSzR#
float4 main(VertexToPixel input) : SV_TARGET {

	float4 output = float4(0,0,0,0);
	
	float2 resolution = float2((float)width, (float)height);
	float2 res = resolution / 2.0;

	float2 fragCoord = input.uv.xy * resolution.xy;//the coordinate in pixels
	
	// Hardness of scanline.
	//  -8.0 = soft
	// -16.0 = medium
	float hardScan = -8.0;

	// Amount of shadow mask.
	float maskDark = 0.5;
	float maskLight = 1.5;

	// Unmodified.
	float2 pos = Warp(input.uv);
	//float2 pos = input.uv;
	//return pixels.SampleLevel(trilinear, pos, 0) - pixels.SampleLevel(trilinear, input.uv, 0);
	//return float4(pos,0,1);
	output.rgb = Tri(pos, res, hardScan) * Mask(fragCoord.xy, maskDark, maskLight) + ScanMask(input.uv.y, -maskDark * 0.05, maskLight * 0.05);
	
	output.rgb = ToSrgb(output.rgb);
	output.a = 1.0;
	return output;
}

