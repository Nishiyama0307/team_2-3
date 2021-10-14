

#include "stage.h"
#include "collision.h"


bool Stage::RayPick(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    if (mdl_stage.get() == nullptr) return false;

    return Collision3D::RayPickVsModel(start, end, mdl_stage.get(), hit);
}