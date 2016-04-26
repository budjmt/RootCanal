#pragma once

#include "PostProcessBase.h"
#include "MarchMath.h"

class OpacityRadius : public PostProcessBase
{
public:
	OpacityRadius();
	~OpacityRadius();
	SRV* draw(vec3 playerPos,ID3D11SamplerState* sampler);

	void setupRenderTarget(RTV** rtv, SRV** srv);
private:
	RTV* renderTarget; //opacity map
	SRV* resourceView;

	SimplePixelShader* ps;
};

