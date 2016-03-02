#include "Material.h"

Material::Material() {}
Material::Material(const wchar_t* tex, ID3D11Device* device, ID3D11DeviceContext* deviceContext) { _texture = Texture::getTexture(tex, device, deviceContext); }
Material::~Material() {}

void Material::vertexShader(SimpleVertexShader* v) { vertex = v; }
void Material::pixelShader(SimplePixelShader* p) { pixel = p; }
void Material::camera(Camera** c) { _camera = c; }
void Material::texture(Texture* t) { _texture = t; }

void Material::updateMaterial(mat4& world) {
	vertex->SetMatrix4x4("world", &world[0][0]);
	vertex->SetMatrix4x4("inv_trans_world", &mat4::inv_tp_tf(world)[0][0]);
	(*_camera)->updateCamMat(vertex);
	if(_texture) _texture->updateTex(pixel);
}

void Material::setActive(bool b) {
	vertex->SetShader(b);
	pixel->SetShader(b);
}

std::map<const wchar_t*, Texture*> Texture::loadedTextures;

Texture::~Texture() {

}

#include "DebugBenchmark.h"
Texture* Texture::getTexture(const wchar_t* texFile, ID3D11Device* device, ID3D11DeviceContext* deviceContext) {
	//check if the texture was already loaded
	if (loadedTextures.find(texFile) != loadedTextures.end())
		return loadedTextures[texFile];

	Texture* texture = new Texture;
	HR(DirectX::CreateWICTextureFromFile(device, deviceContext, texFile, nullptr, &texture->resourceView));

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	HR(device->CreateSamplerState(&desc, &texture->samplerState));
	
	loadedTextures[texFile] = texture;
	return texture;
}

void Texture::updateTex(ISimpleShader* shader) { 
	shader->SetShaderResourceView("diffuseTexture", resourceView);
	shader->SetSamplerState("basicSampler", samplerState); 
}