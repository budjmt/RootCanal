#pragma once
#include "PostProcessBase.h"
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Vertex.h"
class Blur: public PostProcessBase
{
public:
	Blur(UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext,ID3D11SamplerState* _sampler, ID3D11RenderTargetView* _rtv);
	~Blur();

	virtual ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);
	void setBlurAmount(int in);
private:
	// Post process stuff
	ID3D11RenderTargetView* blurRTV;
	ID3D11ShaderResourceView* blurSRV;
	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;

	int blurAmount = 5;
};

