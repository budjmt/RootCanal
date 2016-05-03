#include "OpacityRadius.h"

#include "Shader.h"
#include "DXInfo.h"
#include "Mesh.h"
#include "DrawMesh.h"

OpacityRadius::OpacityRadius(UINT w, UINT h, ID3D11SamplerState* sampler, Camera** cam) : PostProcessBase()
{
	DXInfo& d = DXInfo::getInstance();
	device = d.device;
	deviceContext = d.deviceContext;
	windowWidth = w; windowHeight = h;
	setupRenderTarget(&renderTarget, &resourceView);
	vs = Shader::getShader<SimpleVertexShader>(L"FinalVertex");
	ps = Shader::getShader<SimplePixelShader>(L"OpacityMap");

	opacityGO = new GameObject(new DrawMesh(Mesh::createMesh("../Assets/cube.obj"), nullptr, DXInfo::getInstance().device));
	Texture* t = new Texture;
	t->resourceViews.push_back(resourceView);
	t->samplerState = sampler;
	auto m = new Material(t);
	m->vertexShader(Shader::getShader<SimpleVertexShader>(L"BasicVertex"));
	m->pixelShader(ps);
	m->camera(cam);
	opacityGO->_shape->material(m);
}


OpacityRadius::~OpacityRadius()
{
	//ReleaseMacro(resourceView);
	ReleaseMacro(renderTarget); 
	delete opacityGO->_shape->material();
}

#include "Vertex.h"
SRV* OpacityRadius::draw(vec3 playerPos, ID3D11SamplerState* sampler) {
	RTV* oldTarget;
	deviceContext->OMGetRenderTargets(1, &oldTarget, NULL);
	deviceContext->OMSetRenderTargets(1, &renderTarget, NULL);

	/*UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* nothing = 0;
	deviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	deviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	vs->SetShader();*/

	//THIS LINE IS CAUSING THE PROBLEM. WHY?
	ps->SetShaderResourceView("diffuseTexture", resourceView);
	ps->SetSamplerState("basicSampler", sampler);
	ps->SetFloat4("playerPos", &vec4(playerPos)[0]);
	ps->SetShader();

	//deviceContext->Draw(3, 0);
	opacityGO->draw(deviceContext);
	
	ps->SetShaderResourceView("diffuseTexture", 0);

	deviceContext->OMSetRenderTargets(1, &oldTarget, NULL);
	oldTarget->Release();

	return resourceView;
}

void OpacityRadius::setupRenderTarget(RTV** rtv, SRV** srv) {
	// Create a texture
	D3D11_TEXTURE2D_DESC tDesc = {};
	tDesc.Width  = windowWidth;
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