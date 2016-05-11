#pragma once
#include "DirectXGameCore.h"
#include "MarchMath.h"
#include "SimpleShader.h"
#include "DXInfo.h"

class OpacityCompute
{
public:
	OpacityCompute(ID3D11Device* _device);

	ID3D11ShaderResourceView* getSRV();
	void dispatch(vec3 relativePos);
	~OpacityCompute();
private:
	ID3D11Device* device;
	int textureSize;

	SimpleComputeShader* computeShader;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11UnorderedAccessView* textureUAV;

	ID3D11ShaderResourceView* textureSRV2;
	ID3D11UnorderedAccessView* textureUAV2;

	ID3D11ShaderResourceView** srvArray;
	ID3D11UnorderedAccessView** uavArray;
	void setupTextures();

	SimpleComputeShader* initShader;
	int z = 0;
};

