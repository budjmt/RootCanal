#pragma once

#include <map>
#include "SimpleShader.h"

class Shader
{
public:
    static std::map<const wchar_t*, ISimpleShader*> loadedShaders;//all currently loaded textures
    static ISimpleShader* getShader( const wchar_t* shaderFile );
};