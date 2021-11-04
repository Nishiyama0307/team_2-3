#pragma once

#include "Graphics/Graphics.h"
#include <vector>
#include "Stage.h"

// 追加
#include "stage_type.h"



// ステージ管理クラス ※シングルトンパターン

class StageManager
{
private:
    StageManager() {}
    ~StageManager() {}

public:
    static StageManager& Instance()
    {
        static StageManager instance;
        return instance;
    }

public:
    void Update(float elapsedTime, int stage_num);
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader);

public: // Get関数
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

public: // Set関数
    void AddStage(Stage* stage);

    void AllClear();

private:
    int stage_num = {};
    std::vector<Stage*> stages = {};
};