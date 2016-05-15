#include "CRT.h"

CRT::CRT(UINT width, UINT height, ID3D11Device* _device, ID3D11DeviceContext * _deviceContext, ID3D11SamplerState* _sampler)
	: PostProcessBase()
{
	windowWidth = width;
	windowHeight = height;

	device = _device;
	deviceContext = _deviceContext;

	sampler = _sampler;

	filterVS = new SimpleVertexShader(device, deviceContext);

	filterPS = new SimplePixelShader(device, deviceContext);
	filterPS->LoadShaderFile(L"CRTFilter.cso");

	//Sets up the render targets and srv's for both passes
	setupRenderTarget(&filterRTV, &filterSRV);
}


CRT::~CRT()
{
	ReleaseMacro(filterSRV);
	ReleaseMacro(filterRTV);

	if (filterVS) { delete filterVS; filterVS = nullptr; }
	if (filterPS) { delete filterPS; filterPS = nullptr; }
}

#include "MarchMath.h"
ID3D11ShaderResourceView* CRT::draw(ID3D11ShaderResourceView* ppSRV) {
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &filterRTV, 0);
	deviceContext->ClearRenderTargetView(filterRTV, color);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	filterPS->SetInt("width", windowWidth);
	filterPS->SetInt("height", windowHeight);

	timeElapsed = fmod(GetTickCount64() / 1000.f, PI * 200);
	filterPS->SetFloat("time", timeElapsed);

	filterPS->SetShaderResourceView("pixels", ppSRV);
	filterPS->SetSamplerState("trilinear", sampler);
	filterPS->SetShader();

	deviceContext->Draw(3, 0);
	filterPS->SetShaderResourceView("pixels", 0);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &sampler);

	return filterSRV;
}