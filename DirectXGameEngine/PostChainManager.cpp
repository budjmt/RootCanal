#include "PostChainManager.h"


PostChainManager::PostChainManager(PostProcess* pp, UINT width, UINT height, ID3D11Device* device, ID3D11DeviceContext* deviceContext,ID3D11SamplerState* sampler)
{
	postProcess = pp;

	setChain(width,height,device,deviceContext,sampler);
}

PostChainManager::~PostChainManager()
{
	delete bloom; delete ca; delete edgeDetect; delete crt;
}

void PostChainManager::setChain(UINT width, UINT height, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* sampler) {
	bloom = new Bloom(width, height, device, deviceContext, sampler);
	bloom->setBlurAmount(3);
	bloom->setMinIntensity(0.9f);

	ca = new ChromaticAberation(width, height, device, deviceContext,sampler);
	ca->setDistortAmount(6.f);
    
	edgeDetect = new EdgeDetect(width, height, device, deviceContext);
	edgeDetect->setOutlineWidth(2);

	crt = new CRT(width, height, device, deviceContext,sampler);

	postProcess->AddEffect(0,edgeDetect);
	postProcess->AddEffect(0,ca);
	postProcess->AddEffect(0,bloom);
	postProcess->AddEffect(0, crt);
    
	postProcess->AddEffect(1, crt);

	postProcess->setChain(0);
}
