#pragma once

#include "Graphics/Graphics.h"
#include <vector>
#include "Stage.h"

// �ǉ�
#include "stage_type.h"



// �X�e�[�W�Ǘ��N���X ���V���O���g���p�^�[��

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

public: // Get�֐�
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit);

public: // Set�֐�
    void AddStage(Stage* stage);

    void AllClear();

private:
    int stage_num = {};
    std::vector<Stage*> stages = {};
};