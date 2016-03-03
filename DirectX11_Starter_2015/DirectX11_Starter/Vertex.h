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
	DirectX::XMFLOAT3 Start;
	DirectX::XMFLOAT3 StartColor;
	DirectX::XMFLOAT3 End;
	DirectX::XMFLOAT3 EndColor;
};

struct DebugVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
};