

#include "stageManager.h"
#include "collision.h"



void StageManager::Update(float elapsedTime)
{
    for (auto& stage : stages)
    {
        stage->Update(elapsedTime);
    }
}


void StageManager::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
    for (auto& stage : stages)
    {
        stage->ModelRender(dc, shader);
    }
}


bool StageManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit)
{
    bool result = false;

    // ˆê•Ï”
    HitResult hit_result;
    hit_result.distance = FLT_MAX;

    for (auto& stage : stages)
    {
        if (stage->RayPick(start, end, hit) == false) continue;


        if (hit.distance > hit_result.distance) continue;
        // Å’ZŒğ“_‚ÌŒ‹‰Ê‚Ì‚İ‚ğ•Û‘¶‚·‚é

        hit_result = hit;
        result = true;
    }

    return result;
}


void StageManager::AddStage(Stage* stage_)
{
    if (stage_ == nullptr) return;

    stages.emplace_back(stage_);
}


void StageManager::AllClear()
{
    for (auto& stage : stages)
    {
        if (!stage)continue;
        delete stage;
    }
    stages.clear();
}