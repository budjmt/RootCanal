#include "PostChainManager.h"


PostChainManager::PostChainManager(PostProcess* pp, UINT width, UINT height, ID3D11Device* device, ID3D11DeviceContext* deviceContext,ID3D11SamplerState* sampler)
{
	postProcess = pp;

	setChain(width,height,device,deviceContext,sampler);
}

PostChainManager::~PostChainManager()
{
	delete postProcess;
}

void PostChainManager::setChain(UINT width, UINT height, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* sampler) {
	Bloom* bloomEffect = new Bloom(width, height, device, deviceContext, sampler);
	bloomEffect->setBlurAmount(1);
	bloomEffect->setMinIntensity(0.9f);

	ChromaticAberation* test = new ChromaticAberation(width, height, device, deviceContext,sampler);
	test->setDistortAmount(5.f);
    
	EdgeDetect* test2 = new EdgeDetect(width, height, device, deviceContext);
	test->setDistortAmount(5.f);

	postProcess->AddEffect(test2);
	postProcess->AddEffect(test);
	postProcess->AddEffect(bloomEffect);
	
}
