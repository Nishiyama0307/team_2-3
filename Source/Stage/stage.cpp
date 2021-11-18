

#include "stage.h"
#include "collision.h"


bool Stage::RayPick(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit, int stage_num)
{
    if (stage_type == STAGE2_DESERT && stage_num == STAGE1_VOLCANO)
        return Collision3D::RayPickVsModel(start, end, mdl_stage.get(), hit); // ステージとのレイキャスト

    if (stage_type != stage_num) return false; // 今いるステージ以外は除外

    return Collision3D::RayPickVsModel(start, end, mdl_stage.get(), hit); // ステージとのレイキャスト
    //return false;
}