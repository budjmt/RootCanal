#pragma once
#include "DirectXGameCore.h"
#include "MarchMath.h"
#include "SimpleShader.h"
#include "DXInfo.h"
#include "Material.h"
class OpacityCompute
{
public:
	OpacityCompute(ID3D11Device* _device);

	ID3D11ShaderResourceView* getSRV();
	void dispatch(vec3 relativePos);
	~OpacityCompute();
private:
	Texture* randomTexture;
	ID3D11Device* device;
	int textureSize;

	SimpleComputeShader* computeShader;
	SimpleComputeShader* initShader;

	ID3D11ShaderResourceView* textureSRV;
	ID3D11UnorderedAccessView* textureUAV;

	void setupTextures();
};

