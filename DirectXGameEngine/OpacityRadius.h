#pragma once

#include "PostProcessBase.h"
#include "MarchMath.h"
#include "GameObject.h"

class Camera;
class OpacityRadius : public PostProcessBase
{
public:
	OpacityRadius() {};
	OpacityRadius(UINT w, UINT h,ID3D11SamplerState* sampler,Camera** cam);
	~OpacityRadius();
	SRV* draw(vec3 playerPos,ID3D11SamplerState* sampler);

	void setupRenderTarget(RTV** rtv, SRV** srv);

	SRV* resourceView = nullptr;

	GameObject* opacityGO;
private:
	RTV* renderTarget = nullptr; //opacity map
	SimpleVertexShader* vs;
	SimplePixelShader* ps;
};

