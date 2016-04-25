#include "PostProcessBase.h"



PostProcessBase::PostProcessBase()
{
}


PostProcessBase::~PostProcessBase()
{
}

ID3D11ShaderResourceView* PostProcessBase::draw(SRV* ppSRV){
	return ppSRV;
}

void PostProcessBase::setupRenderTarget(ID3D11RenderTargetView** rtv, SRV** srv) {
	// Create a texture
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width = windowWidth;
	tDesc.Height = windowHeight;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.MipLevels = 1;
	tDesc.MiscFlags = 0;
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* ppTexture;
	device->CreateTexture2D(&tDesc, 0, &ppTexture);

	// Make a render target view for rendering into the texture
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = tDesc.Format;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	device->CreateRenderTargetView(ppTexture, &rtvDesc, rtv);

	//Make an SRV 
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = tDesc.Format;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	device->CreateShaderResourceView(ppTexture, &srvDesc, srv);

	//Get rid of ONE of the texture references
	ppTexture->Release();
}