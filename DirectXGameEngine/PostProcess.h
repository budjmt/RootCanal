#pragma once
#include <vector>
#include "PostProcessBase.h"
#include "Blur.h"
#include "Bloom.h"
#include "Vertex.h"
#include "EdgeDetect.h"
#include "EdgeOutlineDetect.h"
#include "FXAA.h"
#include "ChromaticAberation.h"
#include "CRT.h"
#include "Scene.h"
#include "StateManager.h"
class PostProcess
{
public:
	PostProcess(ID3D11Device* _device,ID3D11DeviceContext* _deviceContext, ID3D11SamplerState* _sampler, ID3D11DepthStencilView* _depthStencilView);
	~PostProcess();
	void draw(ID3D11ShaderResourceView* ppSRV, ID3D11RenderTargetView* renderTargetView, ID3D11RenderTargetView* backBufferView);
	void AddEffect(int i, PostProcessBase* effect);
	void setChain(int i);

    void setSceneType( SceneType type );
private:
	std::vector<PostProcessBase*> ppChain;
	std::vector<PostProcessBase*> normalChain;
    std::vector<PostProcessBase*> outlineChain;
	std::vector<PostProcessBase*> xrayChain;

	SimpleVertexShader* ppVS;
	SimplePixelShader* ppPS;
	ID3D11Device * device;
	ID3D11DeviceContext * deviceContext;
	ID3D11SamplerState* sampler;

	ID3D11DepthStencilView* depthStencilView;

	int chainSwap = 0;
    SceneType sceneType;
    SceneType prevSceneType;
};

