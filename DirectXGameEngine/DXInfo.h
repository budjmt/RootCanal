#pragma once
#include "d3d11.h"

struct DXInfo {
public:
    static DXInfo& getInstance() { static DXInfo instance; return instance; };
    ID3D11Device*				device;
    ID3D11DeviceContext*		deviceContext;
    ID3D11RasterizerState*		rasterState;
    D3D11_RASTERIZER_DESC		rasterDesc;
    ID3D11BlendState*			blendState;
    D3D11_BLEND_DESC			blendDesc;
    IDXGISwapChain*				swapChain;
    ID3D11Texture2D*			depthStencilBuffer;
    ID3D11RenderTargetView*		renderTargetView;
    ID3D11DepthStencilView*		depthStencilView;
    D3D11_VIEWPORT*				viewport;
    D3D_DRIVER_TYPE*			driverType;
    D3D_FEATURE_LEVEL*			featureLevel;
private:
    DXInfo() {};
    ~DXInfo() {};
    DXInfo( const DXInfo& ) = delete;
    void operator=( const DXInfo& ) = delete;
};