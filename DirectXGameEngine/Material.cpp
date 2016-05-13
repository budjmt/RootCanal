#include "Material.h"

Material::Material() {}
Material::Material( Texture* tex ) { _texture = tex; }
Material::~Material() {}

void Material::vertexShader( SimpleVertexShader* v ) { vertex = v; }
void Material::pixelShader( SimplePixelShader* p ) { pixel = p; }
void Material::camera( Camera** c ) { _camera = c; }
void Material::texture( Texture* t ) { _texture = t; }

std::map<const wchar_t*, Material*> Material::loadedMaterials;

void Material::updateMaterial( mat4& world ) {
    vertex->SetMatrix4x4( "world", &world[0][0] );
    vertex->SetMatrix4x4( "inv_trans_world", &mat4::inv_tp_tf( world )[0][0] );
    ( *_camera )->updateCamMat( vertex );
    if( _texture ) _texture->bindTex( pixel );
}

void Material::unbindSRV() {
	if (_texture) {
		_texture->unbindTex(pixel);
	}
}

void Material::setActive( bool b ) {
    vertex->SetShader( b );
    pixel->SetShader( b );
}

Material* Material::getMaterial( const wchar_t* key ) {
    return loadedMaterials.at( key );
}

Material* Material::createMaterial( const wchar_t* key, Texture* texture, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, Camera** camera )
{
    Material* mat = new Material();
    mat->vertexShader( vertexShader );
    mat->pixelShader( pixelShader );
    mat->camera( camera );
    mat->texture( texture );
    loadedMaterials[key] = mat;
	mat->setOpacity(1);
    return mat;
}

std::map<const wchar_t*, Texture*> Texture::loadedTextures;

Texture::~Texture() { 
	for (size_t i = 0, numSRV = resourceViews.size(); i < numSRV; i++) {
		ReleaseMacro(resourceViews[i]);
	}
	ReleaseMacro(samplerState); 
}

Texture* Texture::getTexture( const wchar_t* key ) {
    return loadedTextures.at(key);
}

Texture* Texture::createTexture( const wchar_t* texFile, ID3D11Device* device, ID3D11DeviceContext* deviceContext ) {
    Texture* texture = new Texture;
	texture->resourceViews.push_back(nullptr);
    HR( DirectX::CreateWICTextureFromFile( device, deviceContext, texFile, nullptr, &texture->resourceViews[texture->resourceViews.size() - 1] ) );
	assert(texture->resourceViews.size());

    D3D11_SAMPLER_DESC desc;
    ZeroMemory( &desc, sizeof( desc ) );
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    HR( device->CreateSamplerState( &desc, &texture->samplerState ) );

    loadedTextures[texFile] = texture;
    return texture;
}

void Texture::addTex( ID3D11ShaderResourceView* srv ) {
	resourceViews.push_back(srv);
}

void Texture::bindTex( ISimpleShader* shader ) {
	size_t numTextures = resourceViews.size();
	if (numTextures > 1) {
		for (size_t i = 0; i < numTextures; i++) {
			std::string str = "diffuseTexture" +  std::to_string(i);
			shader->SetShaderResourceView(str.c_str(), resourceViews[i]);
		}
	}
	else shader->SetShaderResourceView("diffuseTexture", resourceViews[0]);
    shader->SetSamplerState( "basicSampler", samplerState );
}

void Texture::unbindTex( ISimpleShader* shader ) {
	size_t numTextures = resourceViews.size();
	if (numTextures > 1) {
		for (size_t i = 0; i < numTextures; i++) {
			std::string str = "diffuseTexture" + std::to_string(i);
			shader->SetShaderResourceView(str.c_str(), 0);
		}
	}
	else shader->SetShaderResourceView("diffuseTexture", 0);
}

void Material::setOpacity(float opacity) {
	pixel->SetFloat("opacity", opacity);
}