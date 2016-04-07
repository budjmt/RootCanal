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

	Vertex() {};
	Vertex(DirectX::XMFLOAT4 c, DirectX::XMFLOAT3 p, DirectX::XMFLOAT3 n, DirectX::XMFLOAT2 u) : Color(c), Position(p), Normal(n), UV(u) {};
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