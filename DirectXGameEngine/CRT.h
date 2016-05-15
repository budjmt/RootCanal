#pragma once
#include "PostProcessBase.h"
class CRT :
	public PostProcessBase
{
public:
	CRT(UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler);
	~CRT();
	ID3D11ShaderResourceView* draw(ID3D11ShaderResourceView* ppSRV);

private:
	// Post process stuff
	ID3D11RenderTargetView* filterRTV;
	ID3D11ShaderResourceView* filterSRV;
	SimpleVertexShader* filterVS;
	SimplePixelShader* filterPS;
};
