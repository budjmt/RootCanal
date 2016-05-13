#pragma once

#include "Game.h"
#include "WICTextureLoader.h"
#include "SimpleShader.h"

#include "MarchMath.h"
#include "Camera.h"
#include "DXInfo.h"

#include <map>

class Texture {
public:
    ~Texture();

    std::vector<ID3D11ShaderResourceView*> resourceViews;
    ID3D11SamplerState* samplerState = nullptr;

    static std::map<const wchar_t*, Texture*> loadedTextures;//all currently loaded textures
    static Texture* getTexture( const wchar_t* key );
    static Texture* createTexture( const wchar_t* texFile, ID3D11Device* device, ID3D11DeviceContext* deviceContext );

	void addTex(ID3D11ShaderResourceView* srv);
    void bindTex( ISimpleShader* shader );
	void unbindTex(ISimpleShader* shader);
};

class Material
{
public:
    Material();
    Material( Texture* tex );
    ~Material();
    void vertexShader( SimpleVertexShader* v );
    void pixelShader( SimplePixelShader* p );
	Texture* texture() { return _texture; };
    void texture( Texture* t );
    void camera( Camera** c );

    void updateMaterial( mat4& world );
	void unbindSRV();
    void setActive( bool b );

	void setOpacity(float opacity);

    static std::map<const wchar_t*, Material*> loadedMaterials;//all currently loaded materials
    static Material* getMaterial( const wchar_t* key );
    static Material* createMaterial( const wchar_t* key, Texture* texture, SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, Camera** camera );

private:
    SimpleVertexShader* vertex;
    SimplePixelShader* pixel;
    Camera** _camera;
    Texture* _texture;
	float opacity = 0;
};
