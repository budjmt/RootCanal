#pragma once

#include <DirectXMath.h>
#include "Mesh.h"
//#include "Material.h"
#include "Vertex.h"
#include "Transform.h"

class Material;

class Entity
{
public:
	Entity();
    Entity( Mesh* meshPtr, Material* materialPtr );
    ~Entity();

    Mesh* GetMeshPointer();
    void SetMeshPointer( Mesh* value );

    DirectX::XMFLOAT4X4 GetWorldMatrix();
    void SetWorldMatrix( DirectX::XMFLOAT4X4 value );

    DirectX::XMFLOAT3 GetOffset();
    void SetOffset( DirectX::XMFLOAT3 value );
    void SetOffset( float x, float y, float z );

    DirectX::XMFLOAT3 GetRotation();
    void SetRotation( DirectX::XMFLOAT3 value );
    void SetRotation( float x, float y, float z );

    DirectX::XMFLOAT3 GetScale();
    void SetScale( DirectX::XMFLOAT3 value );
    void SetScale( float x, float y, float z );

    void MoveAbsolute( float x, float y, float z );
    void Rotate( float x, float y, float z );
    void Scale( float x, float y, float z );

    void Draw( ID3D11DeviceContext* deviceContextPtr, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix );

protected:
    Mesh* _meshPtr;
    Material* _materialPtr;
	Transform transform;

    bool _dirtyWorldMatrix;

    void _PrepareMaterial( DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix );
};

