#pragma once
#include "PostProcessBase.h"
#include "EdgeDetect.h"

class FXAA : public PostProcessBase
{
public:
	FXAA(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler);
	void setBlurAmount(int in);
	virtual SRV* draw(SRV* ppSRV);
	~FXAA();
private:
	EdgeDetect* edges;

	SimplePixelShader* fxaaPS;

	RTV* aaRTV;
	SRV* aaSRV;

	int blurAmount = 10;
};

