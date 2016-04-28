#pragma once
#include "PostProcessBase.h"
#include "SimpleShader.h"
#include "Vertex.h"
class Bloom : public PostProcessBase
{
public:
	Bloom(UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler);
	~Bloom();
	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);
	void setMinIntensity(float in);
	void setBlurAmount(int in);

private:
	// Post process stuff
	ID3D11RenderTargetView* filterRTV;
	ID3D11ShaderResourceView* filterSRV;
	SimpleVertexShader* filterVS;
	SimplePixelShader* filterPS;

	ID3D11RenderTargetView* compositeRTV;
	ID3D11ShaderResourceView* compositeSRV;
	SimplePixelShader* compositePS;

	float minIntensity = 0.5f;
	int blurAmount = 10;
};

