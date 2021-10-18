#pragma once
#include <DirectXMath.h>

template <typename T>
inline void safe_del(T*& p)
{
    if (p != nullptr)
    {
        delete (p);
        (p) = nullptr;
    }
}

struct Pos
{
	float x, y;
};

struct C_Hitbox
{
	float top, left, right, bottom;
	DirectX::XMFLOAT2 size;	
};

bool C_Hitcheck(float top1, float left1, float bottom1, float right1,
	float top2, float left2, float bottom2, float right2);

void C_OffsetBox(float top, float left, float& bottom, float& right, float sizeX, float sizeY);

