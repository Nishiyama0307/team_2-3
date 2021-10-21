#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>

// �ǉ�
#include "Charactor/Player.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/Enemy_Arrangement.h"
#include "pause.h"
#include "coutdown.h"
#include "C_function.h"
#include "CameraController.h"


// �Q�[���V�[��

class Game : public Scene
{
public:
    Game() {}
    ~Game()override{}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

    void DebugRender()override;

private:
    void CameraSet();
    void ChangeScene(float elapsedTime);
    void ClearedSpriteRender(ID3D11DeviceContext* dc);
    void BGMStart();

private:
    std::unique_ptr<Pause> pause = nullptr;
    std::unique_ptr<CountDown> countdown = nullptr;

    Player* player = nullptr;
    Enemy_Arrangement* enemy_Arrangement = nullptr;

    // ����
    std::unique_ptr<Sprite> black_band = nullptr;
    const float black_band_timer_max = 1.4f;
    float black_band_timer = 0.0f;

    bool smallest = false;

    Pos attacSelectPos;
    int attck_select_state = 0;

    //UI�B
    std::unique_ptr<Sprite> HpBar               = nullptr;      //HP
    std::unique_ptr<Sprite> HpBarFrame          = nullptr;      //
    std::unique_ptr<Sprite> StressBar           = nullptr;      //�X�g���X
    std::unique_ptr<Sprite> StressBarFrame      = nullptr;      //
    std::unique_ptr<Sprite> GoldBar             = nullptr;      //����
    std::unique_ptr<Sprite> Minimap             = nullptr;      //�~�j�}�b�v
    std::unique_ptr<Sprite> Castlebar           = nullptr;      //��
    std::unique_ptr<Sprite> CastlebarFrame      = nullptr;      //
    std::unique_ptr<Sprite> AttackSlot          = nullptr;      //�U���X���b�g
    std::unique_ptr<Sprite> AttackSelect        = nullptr;      //�U���I��

    CameraController* cameraController = nullptr;
};