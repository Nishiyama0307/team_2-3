#pragma once

#include "Graphics/Graphics.h"
#include <memory>
struct HitResult;


enum STAGETYPE
{
	STAGE1_VOLCANO,
	STAGE2_DESERT,
	STAGE3_,
	STAGE4_,
    CASTLE,
};

// �X�e�[�W�N���X �����ۃN���X

class Stage
{
public:
    Stage() {}
    virtual ~Stage() {}

public:
    virtual void Update(float elapsedTime) {};

    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)
    {
        shader->Draw(dc, mdl_stage.get());
    }

public: // Get�֐�
    bool RayPick(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit, int stage_num);

protected:
    STAGETYPE stage_type;
    std::unique_ptr<Model> mdl_stage = nullptr;
};