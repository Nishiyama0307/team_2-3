#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>

// �ǉ�
#include "Charactor/Player.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/NormalEnemy.h"
#include "Charactor/BombEnemy.h"
#include "Charactor/Enemy_Arrangement.h"


// �Q�[���V�[��

class Tutorial : public Scene
{
public:
    Tutorial() {}
    ~Tutorial()override {}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

    void DebugRender()override;

    void End_of_explanation(float elapsedTime);

private:
    void CameraSet();

private:
    std::unique_ptr<Sprite> spr_explanation = nullptr;
    std::unique_ptr<Sprite> spr_space = nullptr;
    std::unique_ptr<Sprite> spr_start = nullptr;

    Player* player = nullptr;
    Enemy_Arrangement* enemy_Arrangement = nullptr;

    int tutorial_state = 0;

    float angle = 0.0f;
    float foot_length = 1.5f;

    bool explaining = true; // ������
    int explanation = 0;

    bool ws[2] = { false };
    bool adik[4] = { false };

    enum tutorial_contents 
    {
        PLAYER_MOVE,            // �v���C���[�ړ�����
        CAMERA_MOVE,            // �J��������
        //PLAYER_AND_CAMERA_MOVE, // �v���C���[�{�J��������
        MERIT,                  // �ǂ��L
        DEMERIT,                // �����L
        SHELL_SIZE,             // �傫���֌W(�v���C���[��菬�����L�����z����)
        END,                    // �I��
    };
};
