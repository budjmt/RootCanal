#include "Bloom.h"



Bloom::Bloom(float width, float height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	filterVS = new SimpleVertexShader(device, deviceContext);

	compositePS = new SimplePixelShader(device, deviceContext);
	compositePS->LoadShaderFile(L"BloomComposite.cso");

	filterPS = new SimplePixelShader(device, deviceContext);
	filterPS->LoadShaderFile(L"BloomFilter.cso");

	//Sets up the render targets and srv's for both passes
	setupRenderTarget(&filterRTV, &filterSRV);
	setupRenderTarget(&compositeRTV, &compositeSRV);
}


Bloom::~Bloom()
{
	ReleaseMacro(filterSRV);
	ReleaseMacro(filterRTV);
	ReleaseMacro(compositeRTV);
	ReleaseMacro(compositeSRV);

	delete filterVS;
	delete filterPS;
	delete compositePS;
}

void Bloom::setMinIntensity(float in) { minIntensity = in; }
void Bloom::setBlurAmount(int in) { blurAmount = in; }

ID3D11ShaderResourceView* Bloom::draw(ID3D11ShaderResourceView* ppSRV){
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &filterRTV, 0);
	deviceContext->ClearRenderTargetView(filterRTV, color);

	filterPS->SetFloat("minBrightness", minIntensity);
	filterPS->SetShaderResourceView("pixels", ppSRV);
	filterPS->SetSamplerState("trilinear", sampler);
	filterPS->SetShader();

	deviceContext->Draw(3, 0);
	filterPS->SetShaderResourceView("pixels", 0);

	deviceContext->OMSetRenderTargets(1, &compositeRTV, 0);
	deviceContext->ClearRenderTargetView(compositeRTV, color);

	//return testSRV;

	// Create the sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	compositePS->SetInt("blurAmount", blurAmount);
	compositePS->SetFloat("pixelWidth", 1.0f / windowWidth);
	compositePS->SetFloat("pixelHeight", 1.0f / windowHeight);
	compositePS->SetShaderResourceView("test", filterSRV);
	compositePS->SetShaderResourceView("pixels", ppSRV);
	compositePS->SetSamplerState("trilinear", sampler);
	compositePS->SetShader();

	deviceContext->Draw(3, 0);
	compositePS->SetShaderResourceView("pixels", 0);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	return compositeSRV;
}