#include "FXAA.h"

FXAA::FXAA(UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	fxaaPS = new SimplePixelShader(device, deviceContext);
	fxaaPS->LoadShaderFile(L"FXAA.cso");

	//Sets up the render targets and srv's for both passes
	setupRenderTarget(&aaRTV, &aaSRV);

	edges = new EdgeDetect(width, height, device, deviceContext);
	edges->setBlur(false);
	edges->setOutlineWidth(1.5f);
}


FXAA::~FXAA(){
	ReleaseMacro(aaRTV);
	ReleaseMacro(aaSRV);

	delete edges;
	delete fxaaPS;
}

void FXAA::setBlurAmount(int in) { blurAmount = in; }

SRV* FXAA::draw(SRV* ppSRV) {
	SRV* srv = edges->draw(ppSRV);

	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &aaRTV, 0);
	deviceContext->ClearRenderTargetView(aaRTV, color);

	fxaaPS->SetInt("blurAmount", blurAmount);
	fxaaPS->SetShaderResourceView("pixels", ppSRV);
	fxaaPS->SetShaderResourceView("blur", srv);
	fxaaPS->SetFloat("pixelWidth", 1.f / windowWidth);
	fxaaPS->SetFloat("pixelHeight", 1.f / windowHeight);
	fxaaPS->SetShader();

	deviceContext->Draw(3, 0);

	return aaSRV;
}

