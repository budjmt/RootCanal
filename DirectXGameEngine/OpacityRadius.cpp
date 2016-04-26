#include "OpacityRadius.h"

OpacityRadius::OpacityRadius() : PostProcessBase()
{
	ps->LoadShaderFile(L"OpacityPost.cso");
}


OpacityRadius::~OpacityRadius()
{
	ReleaseMacro(renderTarget); ReleaseMacro(resourceView);
	delete ps;
}

SRV* OpacityRadius::draw(vec3 playerPos, ID3D11SamplerState* sampler) {
	//whatever calls this must bind a VS and give it necessary values

	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };

	deviceContext->OMSetRenderTargets(1, &renderTarget, 0);
	deviceContext->ClearRenderTargetView(renderTarget, color);

	ps->SetShaderResourceView("diffuseTexture", resourceView);
	ps->SetSamplerState("basicSampler", sampler);
	ps->SetFloat4("playerPos", &vec4(playerPos)[0]);
	ps->SetShader();

	deviceContext->Draw(3, 0);

	ps->SetShaderResourceView("diffuseTexture", 0);

	return resourceView;
}

void OpacityRadius::setupRenderTarget(RTV** rtv, SRV** srv) {
	// Create a texture
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width = windowWidth;
	tDesc.Height = windowHeight;
	tDesc.ArraySize = 1;
	tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	tDesc.CPUAccessFlags = 0;
	tDesc.Format = DXGI_FORMAT_R8_UNORM;
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