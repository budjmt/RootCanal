#include "Material.h"

Material::Material() {}
Material::Material(const wchar_t* tex, ID3D11Device* device, ID3D11DeviceContext* deviceContext) { _texture = Texture::getTexture(tex, device, deviceContext); }
Material::~Material() {}

void Material::vertexShader(SimpleVertexShader* v) { vertex = v; }
void Material::pixelShader(SimplePixelShader* p) { pixel = p; }
void Material::camera(Camera** c) { _camera = c; }

void Material::updateMaterial(mat4& world) {
	vertex->SetMatrix4x4("world", &world[0][0]);
	vertex->SetMatrix4x4("inv_trans_world", &mat4::inv_tp_tf(world)[0][0]);
	(*_camera)->updateCamMat(vertex);
}

void Material::setActive(bool b) {
	vertex->SetShader(b);
	pixel->SetShader(b);
}

std::map<const wchar_t*, Texture*> Texture::loadedTextures;

Texture* Texture::getTexture(const wchar_t* texFile, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	//check if the texture was already loaded
	if (Texture::loadedTextures.find(texFile) != Texture::loadedTextures.end())
		return Texture::loadedTextures[texFile];

	Texture* texture = new Texture;
	HRESULT(DirectX::CreateWICTextureFromFile(device, deviceContext, texFile, nullptr, &texture->resourceView));

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT(device->CreateSamplerState(&desc, &texture->samplerState));

	return texture;
}

void Texture::updateText(ISimpleShader* shader) { 
	shader->SetSamplerState("diffuseTexture", samplerState);
	shader->SetShaderResourceView("basicSampler", resourceView); 
}