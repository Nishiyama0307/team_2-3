#pragma once

#include "Graphics/Graphics.h"
#include <memory>
struct HitResult;



// ステージクラス ※抽象クラス

class Stage
{
public:
    Stage() = default;
    virtual ~Stage() {};

public:
    virtual void Update(float elapsedTime) {};

    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)
    {
        if (mdl_stage.get() == nullptr) return;

        shader->Draw(dc, mdl_stage.get());
    }

public: // Get関数
    bool RayPick(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

protected:
    std::unique_ptr<Model> mdl_stage = nullptr;
};