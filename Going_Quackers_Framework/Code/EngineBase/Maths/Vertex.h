#pragma once
#include <DirectXMath.h>

struct Vertex2D
{
	Vertex2D() {}
	Vertex2D(float x, float y, float u, float v) : position(x, y, 0), texCoord(u, v) {}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texCoord;
};
