#pragma once

#include "PostProcessBase.h"
#include "MarchMath.h"

class OpacityRadius : public PostProcessBase
{
public:
	OpacityRadius() {};
	OpacityRadius(UINT w, UINT h);
	~OpacityRadius();
	SRV* draw(vec3 playerPos,ID3D11SamplerState* sampler);

	void setupRenderTarget(RTV** rtv, SRV** srv);

	SRV* resourceView = nullptr;
private:
	RTV* renderTarget = nullptr; //opacity map
	SimplePixelShader* ps;
};

