#pragma once
#include "DirectXGameCore.h"
#include "SimpleShader.h"
typedef ID3D11ShaderResourceView SRV;
typedef ID3D11RenderTargetView RTV;

class PostProcessBase
{
public:
	PostProcessBase();
	~PostProcessBase();
	virtual SRV* draw(SRV* ppSRV);

    bool disabled();
    void disabled( bool value );
protected:
	UINT windowWidth, windowHeight;

	virtual void setupRenderTarget(RTV** rtv, SRV** srv);

	ID3D11Device* device;
	ID3D11DeviceContext * deviceContext;
	ID3D11SamplerState* sampler;

    bool _disabled;
};

