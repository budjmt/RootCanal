#pragma once

#include <DirectXMath.h>
#include "Game.h"
#include "SimpleShader.h"

#include "Transform.h"
#include "Material.h"

const int FLOATS_PER_VERT = 3;
const int FLOATS_PER_NORM = 3;
const int FLOATS_PER_UV = 2;

class Drawable
{
public:
    Drawable(); virtual ~Drawable();
    Material* material() const; void material( Material* m );
    virtual void draw( float x, float y, float z, float xScale, float yScale, float zScale, ID3D11DeviceContext* deviceContext );
    virtual void draw( vec3 pos, vec3 scale, vec3 rotAxis, float rot, ID3D11DeviceContext* deviceContext );
    virtual void draw( Transform* t, ID3D11DeviceContext* deviceContext );
    mat4 genWorldMatrix( vec3 pos, vec3 scale, vec3 rotAxis, float rot );

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

protected:
    // Wrappers for DirectX shaders to provide simplified functionality
    Material* _material;
};