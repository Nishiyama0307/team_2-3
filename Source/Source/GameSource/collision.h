#pragma once

#include "Graphics/Graphics.h"



// レイピック用判定結果

struct HitResult
{
    DirectX::XMFLOAT3 position = { 0,0,0 }; // 当てたレイキャストと当たったポリゴンの交点
    DirectX::XMFLOAT3 normal = { 0,0,0 };   // 当たったポリゴンの法線ベクトル
    float distance = 0.0f;                  // レイキャストの始点~終点までの距離
    int materialIndex = -1;                 // 当たったポリゴンのマテリアル番号
    DirectX::XMFLOAT3 rotation = { 0,0,0 }; // 回転量
};




// コリジョンクラス (衝突判定)

class Collision3D
{
public:

    // A押す方、B押し出される方
    static bool BallVsBallAndExtrusion(
        const DirectX::XMFLOAT3& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT3& pos_B,
        const float radius_B,
        DirectX::XMFLOAT3& out_pos_B
    );

    // 球と球の交差判定
    static bool IntersectSqhereVsSqhere(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        DirectX::XMFLOAT3& outPositionB
    );

    // 円柱と円柱の交差判定
    static bool IntersectCylinderVsCylinder(
        const DirectX::XMFLOAT3& positionA,
        float radiusA,
        float heightA,
        const DirectX::XMFLOAT3& positionB,
        float radiusB,
        float heightB,
        DirectX::XMFLOAT3& outPositionB
    );

    // 球と円柱の交差判定
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

    // 円と円の判定
    static bool CircleVsCircle(
        const DirectX::XMFLOAT2& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT2& pos_B,
        const float radius_B
    );

    // 円と円の判定と押し出し
    static bool CircleVsCircleAndExtrusion(
        const DirectX::XMFLOAT2& pos_A,
        const float radius_A,
        const DirectX::XMFLOAT2& pos_B,
        const float radius_B,
        DirectX::XMFLOAT2& out_pos_B
    );

    // 矩形と矩形の判定 ※回転非対応
    static bool RectVsRect(
        const DirectX::XMFLOAT2& rect_A_center_pos,
        const DirectX::XMFLOAT2& rect_A_size,
        const DirectX::XMFLOAT2& rect_B_center_pos,
        const DirectX::XMFLOAT2& rect_B_size
    );

    // 未完成
    // 矩形と矩形の判定と押し出し ※回転非対応
    static bool RectVsRectAndExtrusion(
        const DirectX::XMFLOAT2& rect_A_center_pos,
        const DirectX::XMFLOAT2& rect_A_size,
        const DirectX::XMFLOAT2& rect_B_center_pos,
        const DirectX::XMFLOAT2& rect_B_size,
        DirectX::XMFLOAT2& rect_B_out_center_pos
    );

    // 矩形と円の判定と押し出し　※回転非対応
    static bool RectVsCircleAndExtrusion(
        const DirectX::XMFLOAT2 rect_center_pos,
        const DirectX::XMFLOAT2 rect_size,
        const DirectX::XMFLOAT2 circle_center_pos,
        const float circle_radius,
        DirectX::XMFLOAT2& circle_out_center_pos);

};