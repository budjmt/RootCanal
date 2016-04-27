#pragma once

#include <map>
#include "SimpleShader.h"

class Shader
{
public:
    static std::map<const wchar_t*, ISimpleShader*> loadedShaders;//all currently loaded textures

    template <class type>
    static type* getShader( const wchar_t* key )
    {
        return static_cast<type*>( loadedShaders.at( key ) );
    }

    template <class type>
    static ISimpleShader* createShader( const wchar_t* key, const wchar_t* shaderFile, ID3D11Device* device, ID3D11DeviceContext* deviceContext )
    {
        ISimpleShader* shader = new type( device, deviceContext );
        assert( shader->LoadShaderFile( shaderFile ) );
        loadedShaders[key] = shader;
        return shader;
    }
};