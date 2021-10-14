#pragma once

#include "Graphics/Graphics.h"



// �p���\���p�֗̕��N���X  �� �S��public

class Transform
{
public:
    Transform() {};
    virtual ~Transform() {};

public: // Get�֐�

    // �ʒu //
    float PositionX() { return position.x; }
    float PositionY() { return position.y; }
    float PositionZ() { return position.z; }

    // �傫�� //
    float ScaleX() { return scale.x; }
    float ScaleY() { return scale.y; }
    float ScaleZ() { return scale.z; }

    // ���p�x //
    float AngleX() { return angle.x; }
    float AngleY() { return angle.y; }
    float AngleZ() { return angle.z; }

    // �p������ //
    DirectX::XMFLOAT3 Right()   { return DirectX::XMFLOAT3(matrix_transform._11, matrix_transform._12, matrix_transform._13); }
    DirectX::XMFLOAT3 Up()      { return DirectX::XMFLOAT3(matrix_transform._21, matrix_transform._22, matrix_transform._23); }
    DirectX::XMFLOAT3 Front()   { return DirectX::XMFLOAT3(matrix_transform._31, matrix_transform._32, matrix_transform._33); }


public: // Set�֐�

    // �p���s�񂩂烏�[���h�s��ւ̕ϊ� //
    void ConvertToWorldMatrixFromMatrixTransform()
    {
        DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

        DirectX::XMMATRIX W = S * R * T;

        DirectX::XMStoreFloat4x4(&matrix_transform, W);
    }

    // �C�ӂ̏����Ń��[���h�s��ւ̕ϊ� //
    virtual void ConvertToWorldMatrix_Customize() {};

public: // �ϐ�
    DirectX::XMFLOAT3 position = {};            // �ʒu
    DirectX::XMFLOAT3 scale = {};               // �傫��
    DirectX::XMFLOAT3 angle = {};               // ���p�x

    DirectX::XMFLOAT4X4 matrix_transform = {    // �p���s��
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
};