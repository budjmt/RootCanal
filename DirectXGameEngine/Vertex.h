#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition - You will eventually ADD TO this!
// --------------------------------------------------------
struct Vertex
{
    DirectX::XMFLOAT4 Color;        // The color of the vertex
    DirectX::XMFLOAT3 Position;	    // The position of the vertex
    DirectX::XMFLOAT3 Normal;
    DirectX::XMFLOAT2 UV;
};

struct DebugVector {
    DirectX::XMFLOAT4 Position;
    DirectX::XMFLOAT4 Color;
};

struct DebugMesh {
    DirectX::XMFLOAT4X4 World;
    DirectX::XMFLOAT4 Color;
};

struct DebugVertex
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT3 Normal;
};