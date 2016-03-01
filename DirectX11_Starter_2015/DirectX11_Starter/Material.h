#pragma once

#include "DirectXGameCore.h"
#include "WICTextureLoader.h"
#include "SimpleShader.h"

#include "MarchMath.h"
#include "Camera.h"

#include <map>

class Texture {
	public:
		~Texture();

		ID3D11ShaderResourceView* resourceView = nullptr;
		ID3D11SamplerState* samplerState = nullptr;

		static std::map<const wchar_t*, Texture*> loadedTextures;//all currently loaded textures
		static Texture* getTexture(const wchar_t* texFile, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

		void updateTex(ISimpleShader* shader);
};

class Material
{
public:
	Material();
	Material(const wchar_t* tex, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Material();
	void vertexShader(SimpleVertexShader* v); 
	void pixelShader(SimplePixelShader* p);
	void texture(Texture* t);
	void camera(Camera** c);

	void updateMaterial(mat4& world);
	void setActive(bool b);

private:
	SimpleVertexShader* vertex;
	SimplePixelShader* pixel;
	Camera** _camera;
	Texture* _texture;
};

