#include "Blur.h"



Blur::Blur(UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler, ID3D11RenderTargetView* _rtv)
	: PostProcessBase()
{
	device = _device;
	deviceContext = _deviceContext;

	windowWidth = width;
	windowHeight = height;

	sampler = _sampler;

	ppVS = new SimpleVertexShader(device, deviceContext);
	ppVS->LoadShaderFile(L"BlurVS.cso");

	ppPS = new SimplePixelShader(device, deviceContext);
	ppPS->LoadShaderFile(L"BlurPS.cso");

	//blurRTV = _rtv;

	setupRenderTarget(&blurRTV, &blurSRV);
}

Blur::~Blur()
{
	ReleaseMacro(blurRTV);
	ReleaseMacro(blurSRV);

	delete ppVS;
	delete ppPS;
}

void Blur::setBlurAmount(int in) { blurAmount = in; }

ID3D11ShaderResourceView* Blur::draw(ID3D11ShaderResourceView* ppSRV){

	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &blurRTV, 0);
	deviceContext->ClearRenderTargetView(blurRTV, color);

	ppPS->SetInt("blurAmount", blurAmount);
	ppPS->SetFloat("pixelWidth", 1.0f / windowWidth);
	ppPS->SetFloat("pixelHeight", 1.0f / windowHeight);
	ppPS->SetShaderResourceView("pixels", ppSRV);
	ppPS->SetSamplerState("trilinear", sampler);
	ppPS->SetShader();

	deviceContext->Draw(3, 0);
	ppPS->SetShaderResourceView("pixels", 0);

	return blurSRV;
}
