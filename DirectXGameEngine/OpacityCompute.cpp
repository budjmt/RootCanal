#include "OpacityCompute.h"



OpacityCompute::OpacityCompute(ID3D11Device* _device)
{
	device = _device;
	setupTextures();

	computeShader = new SimpleComputeShader(DXInfo::getInstance().device, DXInfo::getInstance().deviceContext);
	computeShader->LoadShaderFile(L"Opacity.cso");
}


OpacityCompute::~OpacityCompute()
{
}

ID3D11ShaderResourceView* OpacityCompute::getSRV() {
	return textureSRV;
}

void OpacityCompute::dispatch(vec3 relativePos) {
	
	computeShader->SetInt("width", textureSize);
	computeShader->SetUnorderedAccessView("outTex", textureUAV);
	//computeShader->SetFloat2("relativePos", { relativePos.x,relativePos.y});
	computeShader->SetShader(true);
	computeShader->DispatchByThreads(textureSize, textureSize, 1);

	computeShader->SetUnorderedAccessView("outTex", 0);
}

void OpacityCompute::setupTextures() {
	// Create texture for compute shader
	textureSize = 512;
	ID3D11Texture2D* tex;

	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = textureSize;
	texDesc.Height = textureSize;
	texDesc.ArraySize = 1;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	device->CreateTexture2D(&texDesc, 0, &tex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(tex, &srvDesc, &textureSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.Format = texDesc.Format;
	uavDesc.Texture2D.MipSlice = 0;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	device->CreateUnorderedAccessView(tex, &uavDesc, &textureUAV);

	tex->Release();
}