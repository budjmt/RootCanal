#pragma once
#include "PostProcess.h"
#include <map>

class PostChainManager
{
public:
	PostChainManager(PostProcess* pp, UINT width, UINT height, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* sampler);
	~PostChainManager();
	void setChain(UINT width, UINT height, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11SamplerState* sampler);

private:
	PostProcess* postProcess;
	std::map<char*, int> ppChainReference;
	Bloom* bloom;
	ChromaticAberation* ca;
	EdgeDetect* edgeDetect;
    EdgeOutlineDetect* edgeOutlineDetect;
	CRT* crt;
};

