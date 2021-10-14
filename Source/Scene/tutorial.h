#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>

// 追加
#include "Charactor/Player.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/NormalEnemy.h"
#include "Charactor/BombEnemy.h"
#include "Charactor/Enemy_Arrangement.h"


// ゲームシーン

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

    bool explaining = true; // 説明中
    int explanation = 0;

    bool ws[2] = { false };
    bool adik[4] = { false };

    enum tutorial_contents 
    {
        PLAYER_MOVE,            // プレイヤー移動操作
        CAMERA_MOVE,            // カメラ操作
        //PLAYER_AND_CAMERA_MOVE, // プレイヤー＋カメラ操作
        MERIT,                  // 良い貝
        DEMERIT,                // 悪い貝
        SHELL_SIZE,             // 大きさ関係(プレイヤーより小さい貝だけ吸える)
        END,                    // 終了
    };
};
