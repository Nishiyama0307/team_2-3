#pragma once

#include "Graphics/Graphics.h"



// 姿勢表現用の便利クラス  ※ 全てpublic

class Transform
{
public:
    Transform() {};
    virtual ~Transform() {};

public: // Get関数

    // 位置 //
    float PositionX() { return position.x; }
    float PositionY() { return position.y; }
    float PositionZ() { return position.z; }

    // 大きさ //
    float ScaleX() { return scale.x; }
    float ScaleY() { return scale.y; }
    float ScaleZ() { return scale.z; }

    // 軸角度 //
    float AngleX() { return angle.x; }
    float AngleY() { return angle.y; }
    float AngleZ() { return angle.z; }

    // 姿勢方向 //
    DirectX::XMFLOAT3 Right()   { return DirectX::XMFLOAT3(matrix_transform._11, matrix_transform._12, matrix_transform._13); }
    DirectX::XMFLOAT3 Up()      { return DirectX::XMFLOAT3(matrix_transform._21, matrix_transform._22, matrix_transform._23); }
    DirectX::XMFLOAT3 Front()   { return DirectX::XMFLOAT3(matrix_transform._31, matrix_transform._32, matrix_transform._33); }


public: // Set関数

    // 姿勢行列からワールド行列への変換 //
    void ConvertToWorldMatrixFromMatrixTransform()
    {
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

        DirectX::XMMATRIX W = S * R * T;

        DirectX::XMStoreFloat4x4(&matrix_transform, W);
    }

    // 任意の処理でワールド行列への変換 //
    virtual void ConvertToWorldMatrix_Customize() {};

public: // 変数
    DirectX::XMFLOAT3 position = {};            // 位置
    DirectX::XMFLOAT3 scale = {};               // 大きさ
    DirectX::XMFLOAT3 angle = {};               // 軸角度

    DirectX::XMFLOAT4X4 matrix_transform = {    // 姿勢行列
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};