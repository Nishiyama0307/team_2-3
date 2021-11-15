#pragma once

#include "Graphics/Graphics.h"

inline DirectX::XMFLOAT3 float3SUM(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
    return DirectX::XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline DirectX::XMFLOAT3 float3Scaling(DirectX::XMFLOAT3 a, float scale)
{
    return DirectX::XMFLOAT3(a.x * scale, a.y * scale, a.z * scale);
}

inline float Lerq(float a, float b, float t)
{
    return a * (1.0f - t) + (b * t);
}