#pragma once
#include "PostProcess.h"
#include <map>
class PostChainManager
{
public:
	PostChainManager(PostProcess* pp, float width, float height, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* sampler);
	~PostChainManager();
	void setChain(float width, float height, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* sampler);

private:
	PostProcess* postProcess;
	std::map<char*, int> ppChainReference;
};

