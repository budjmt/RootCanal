#include "OpacityCompute.h"



OpacityCompute::OpacityCompute(ID3D11Device* _device)
{
	device = _device;
	setupTextures();

	computeShader = new SimpleComputeShader(DXInfo::getInstance().device, DXInfo::getInstance().deviceContext);
	computeShader->LoadShaderFile(L"Opacity.cso");

	initShader = new SimpleComputeShader(DXInfo::getInstance().device, DXInfo::getInstance().deviceContext);
	initShader->LoadShaderFile(L"InitTexture.cso");


	initShader->SetUnorderedAccessView("outTex", uavArray[(z++) % 2]);
	initShader->SetShader(true);
	initShader->DispatchByThreads(textureSize, textureSize, 1);
	initShader->SetUnorderedAccessView("outTex", 0);
}


OpacityCompute::~OpacityCompute()
{
}

ID3D11ShaderResourceView* OpacityCompute::getSRV() {
	return srvArray[z];
}

void OpacityCompute::dispatch(vec3 relativePos) {
	
	computeShader->SetInt("width", textureSize);
	computeShader->SetUnorderedAccessView("outTex", uavArray[(z++)%2]);
	//computeShader->SetUnorderedAccessView("readTex", uavArray[(z++)%2]);
	//computeShader->SetFloat2("relativePos", { relativePos.x,relativePos.y});
	computeShader->SetFloat2("relativePos", { relativePos.x,relativePos.y });
	computeShader->SetShader(true);
	computeShader->DispatchByThreads(textureSize, textureSize, 1);

	computeShader->SetUnorderedAccessView("outTex", 0);
}

void OpacityCompute::setupTextures() {
	// Create texture for compute shader
	textureSize = 2048;
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

	ID3D11Texture2D* tex2;

	D3D11_TEXTURE2D_DESC texDesc2 = {};
	texDesc2.Width = textureSize;
	texDesc2.Height = textureSize;
	texDesc2.ArraySize = 1;
	texDesc2.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc2.CPUAccessFlags = 0;
	texDesc2.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc2.MipLevels = 1;
	texDesc2.MiscFlags = 0;
	texDesc2.SampleDesc.Count = 1;
	texDesc2.SampleDesc.Quality = 0;
	texDesc2.Usage = D3D11_USAGE_DEFAULT;
	device->CreateTexture2D(&texDesc2, 0, &tex2);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc2 = {};
	srvDesc2.Format = texDesc.Format;
	srvDesc2.Texture2D.MipLevels = 1;
	srvDesc2.Texture2D.MostDetailedMip = 0;
	srvDesc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(tex, &srvDesc2, &textureSRV2);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc2 = {};
	uavDesc2.Format = texDesc.Format;
	uavDesc2.Texture2D.MipSlice = 0;
	uavDesc2.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	device->CreateUnorderedAccessView(tex2, &uavDesc2, &textureUAV2);

	tex2->Release();

	uavArray = new ID3D11UnorderedAccessView*[2];
	uavArray[0] = textureUAV;
	uavArray[1] = textureUAV2;

	srvArray = new ID3D11ShaderResourceView*[2];
	srvArray[0] = textureSRV;
	srvArray[1] = textureSRV2;
}