#include "ChromaticAberation.h"



ChromaticAberation::ChromaticAberation(UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	caPS = new SimplePixelShader(device, deviceContext);
	caPS->LoadShaderFile(L"ChromaticAberation.cso");

	//Sets up the render targets and srv's for both passes
	setupRenderTarget(&caRTV, &caSRV);

	sampler = _sampler;

	amount = 8.f;
}


ChromaticAberation::~ChromaticAberation()
{
	caSRV->Release();
	caRTV->Release();
	delete caPS;
}

void ChromaticAberation::setDistortAmount(float in) { amount = in; }

SRV* ChromaticAberation::draw(SRV* ppSRV) {
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	// Create the sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	deviceContext->OMSetRenderTargets(1, &caRTV, 0);
	deviceContext->ClearRenderTargetView(caRTV, color);

	caPS->SetFloat("distortAmount", amount);
	caPS->SetFloat("pixelWidth", 1.f / windowWidth);
	caPS->SetFloat("pixelHeight", 1.f / windowHeight);
	caPS->SetShaderResourceView("pixels",ppSRV);
	caPS->SetShader();

	deviceContext->Draw(3, 0);

	caPS->SetShaderResourceView("pixels", 0);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	return caSRV;
}