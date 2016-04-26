#include "PostChainManager.h"


PostChainManager::PostChainManager(PostProcess* pp, float width, float height, ID3D11Device* device, ID3D11DeviceContext* deviceContext,ID3D11SamplerState* sampler)
{
	postProcess = pp;

	setChain(width,height,device,deviceContext,sampler);
}

PostChainManager::~PostChainManager()
{
	delete postProcess;
}

void PostChainManager::setChain(float width, float height, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* sampler) {
	Bloom* bloomEffect = new Bloom(width, height, device, deviceContext, sampler);
	bloomEffect->setBlurAmount(2);
	ChromaticAberation* test = new ChromaticAberation(width, height, device, deviceContext,sampler);
	postProcess->AddEffect(test);
	postProcess->AddEffect(bloomEffect);
}
