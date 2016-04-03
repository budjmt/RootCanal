#include "Entity.h"

Entity::Entity( Mesh* meshPtr, Material* materialPtr )
{
    _meshPtr = meshPtr;
    _materialPtr = materialPtr;

    _offset = DirectX::XMFLOAT3( 0, 0, 0 );
    _rotation = DirectX::XMFLOAT3( 0, 0, 0 );
    _scale = DirectX::XMFLOAT3( 1, 1, 1 );

    _dirtyWorldMatrix = true;
}

Entity::~Entity()
{
}

Mesh* Entity::GetMeshPointer()
{
    return _meshPtr;
}

void Entity::SetMeshPointer( Mesh* value )
{
    _meshPtr = value;
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix()
{
    return _worldMatrix;
}

void Entity::SetWorldMatrix( DirectX::XMFLOAT4X4 value )
{
    _worldMatrix = value;
}

DirectX::XMFLOAT3 Entity::GetOffset()
{
    return _offset;
}

void Entity::SetOffset( DirectX::XMFLOAT3 value )
{
    _offset = value;
    _dirtyWorldMatrix = true;
}

void Entity::SetOffset( float x, float y, float z )
{
    _offset.x = x;
    _offset.y = y;
    _offset.z = z;
    _dirtyWorldMatrix = true;
}

DirectX::XMFLOAT3 Entity::GetRotation()
{
    return _rotation;
}

void Entity::SetRotation( DirectX::XMFLOAT3 value )
{
    _rotation = value;
    _dirtyWorldMatrix = true;
}

void Entity::SetRotation( float x, float y, float z )
{
    _rotation.x = x;
    _rotation.y = y;
    _rotation.z = z;
    _dirtyWorldMatrix = true;
}

DirectX::XMFLOAT3 Entity::GetScale()
{
    return _scale;
}

void Entity::SetScale( DirectX::XMFLOAT3 value )
{
    _scale = value;
    _dirtyWorldMatrix = true;
}

void Entity::SetScale( float x, float y, float z )
{
    _scale.x = x;
    _scale.y = y;
    _scale.z = z;
    _dirtyWorldMatrix = true;
}

void Entity::MoveAbsolute( float x, float y, float z )
{
    _offset.x += x;
    _offset.y += y;
    _offset.z += z;
    _dirtyWorldMatrix = true;
}

void Entity::Rotate( float x, float y, float z )
{
    _rotation.x += x;
    _rotation.y += y;
    _rotation.z += z;
    _dirtyWorldMatrix = true;
}

void Entity::Scale( float x, float y, float z )
{
    _scale.x *= x;
    _scale.y *= y;
    _scale.z *= z;
    _dirtyWorldMatrix = true;
}

void Entity::Draw( ID3D11DeviceContext* deviceContextPtr, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix )
{
    // Only update the world matrix when we need to
    if( _dirtyWorldMatrix == true )
    {
        // Create individual transformtaions
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation( _offset.x, _offset.y, _offset.z );
        DirectX::XMMATRIX rotation    = DirectX::XMMatrixRotationY( _rotation.y );
        DirectX::XMMATRIX scale       = DirectX::XMMatrixScaling( _scale.x, _scale.y, _scale.z );

        // Combine together
        DirectX::XMMATRIX world = scale * rotation * translation;

        // Transpose and store
        DirectX::XMStoreFloat4x4( &_worldMatrix, DirectX::XMMatrixTranspose( world ) );

        // We've updated the world matrix, so it's no longer dirty
        _dirtyWorldMatrix = false;
    }

    UINT stride = sizeof( Vertex );
    UINT offset = 0;

    ID3D11Buffer* vertexBuffer = _meshPtr->GetVertexBuffer();
    ID3D11Buffer* indexBuffer = _meshPtr->GetIndexBuffer();

    deviceContextPtr->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
    deviceContextPtr->IASetIndexBuffer( indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

    _PrepareMaterial( viewMatrix, projectionMatrix );

    deviceContextPtr->DrawIndexed(
        _meshPtr->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
        0,                             // Offset to the first index we want to use
        0                              // Offset to add to each index when looking up vertices
    );

    SimpleVertexShader* vertexShader = _materialPtr->GetVertexShaderPtr();
    vertexShader->CopyAllBufferData();
}

void Entity::_PrepareMaterial( DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix )
{
    SimpleVertexShader* vertexShader = _materialPtr->GetVertexShaderPtr();
    SimplePixelShader* pixelShader = _materialPtr->GetPixelShaderPtr();
    vertexShader->SetMatrix4x4( "view", viewMatrix );
    vertexShader->SetMatrix4x4( "projection", projectionMatrix );
    vertexShader->SetMatrix4x4( "world", _worldMatrix );

    vertexShader->SetShader( true );
    pixelShader->SetShader( true );
}
