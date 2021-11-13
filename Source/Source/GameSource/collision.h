#pragma once

#include "Graphics/Graphics.h"



// ���C�s�b�N�p���茋��

struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 }; // ���Ă����C�L���X�g�Ɠ��������|���S���̌�_
    DirectX::XMFLOAT3 normal = { 0,0,0 };   // ���������|���S���̖@���x�N�g��
    float distance = 0.0f;                  // ���C�L���X�g�̎n�_~�I�_�܂ł̋���
    int materialIndex = -1;                 // ���������|���S���̃}�e���A���ԍ�
    DirectX::XMFLOAT3 rotation = { 0,0,0 }; // ��]��
};




// �R���W�����N���X (�Փ˔���)

class Collision3D
{
public:

    // A�������AB�����o������
    static bool BallVsBallAndExtrusion(
        const DirectX::XMFLOAT3& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT3& pos_B,
        const float radius_B,
        DirectX::XMFLOAT3& out_pos_B
    );

    // ���Ƌ��̌�������
    static bool IntersectSqhereVsSqhere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    // �~���Ɖ~���̌�������
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    // ���Ɖ~���̌�������
    static bool IntersectSqhereVsCylinder(
        const DirectX::XMFLOAT3& sqherePosition,
        float sqhereRadius,
        const DirectX::XMFLOAT3& cylinderPosition,
        float cylinderRadius,
        float cylinderHeight,
        DirectX::XMFLOAT3& outCylinderPosition
    );

    static bool RayPickVsModel(
        const DirectX::XMFLOAT3& start,
        const DirectX::XMFLOAT3& end,
        const Model* model,
        HitResult& result
    );
};


class Collision2D
{
public:

    // �~�Ɖ~�̔���
    static bool CircleVsCircle(
        const DirectX::XMFLOAT2& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT2& pos_B,
        const float radius_B
    );

    // �~�Ɖ~�̔���Ɖ����o��
    static bool CircleVsCircleAndExtrusion(
        const DirectX::XMFLOAT2& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT2& pos_B,
        const float radius_B,
        DirectX::XMFLOAT2& out_pos_B
    );

    // ��`�Ƌ�`�̔��� ����]��Ή�
    static bool RectVsRect(
        const DirectX::XMFLOAT2& rect_A_center_pos,
        const DirectX::XMFLOAT2& rect_A_size,
        const DirectX::XMFLOAT2& rect_B_center_pos,
        const DirectX::XMFLOAT2& rect_B_size
    );

    // ������
    // ��`�Ƌ�`�̔���Ɖ����o�� ����]��Ή�
    static bool RectVsRectAndExtrusion(
        const DirectX::XMFLOAT2& rect_A_center_pos,
        const DirectX::XMFLOAT2& rect_A_size,
        const DirectX::XMFLOAT2& rect_B_center_pos,
        const DirectX::XMFLOAT2& rect_B_size,
        DirectX::XMFLOAT2& rect_B_out_center_pos
    );

    // ��`�Ɖ~�̔���Ɖ����o���@����]��Ή�
    static bool RectVsCircleAndExtrusion(
        const DirectX::XMFLOAT2 rect_center_pos,
        const DirectX::XMFLOAT2 rect_size,
        const DirectX::XMFLOAT2 circle_center_pos,
        const float circle_radius,
        DirectX::XMFLOAT2& circle_out_center_pos);

};