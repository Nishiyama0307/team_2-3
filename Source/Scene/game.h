#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>

// 追加
#include "Charactor/Player.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/Enemy_Arrangement.h"
#include "pause.h"
#include "coutdown.h"
#include "C_function.h"
#include "CameraController.h"


// ゲームシーン

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

    // 黒帯
    std::unique_ptr<Sprite> black_band = nullptr;
    const float black_band_timer_max = 1.4f;
    float black_band_timer = 0.0f;

    bool smallest = false;

    Pos attacSelectPos;
    int attck_select_state = 0;

    //UI達
    std::unique_ptr<Sprite> HpBar               = nullptr;      //HP
    std::unique_ptr<Sprite> HpBarFrame          = nullptr;      //
    std::unique_ptr<Sprite> StressBar           = nullptr;      //ストレス
    std::unique_ptr<Sprite> StressBarFrame      = nullptr;      //
    std::unique_ptr<Sprite> GoldBar             = nullptr;      //お金
    std::unique_ptr<Sprite> Minimap             = nullptr;      //ミニマップ
    std::unique_ptr<Sprite> Castlebar           = nullptr;      //城
    std::unique_ptr<Sprite> CastlebarFrame      = nullptr;      //
    std::unique_ptr<Sprite> AttackSlot          = nullptr;      //攻撃スロット
    std::unique_ptr<Sprite> AttackSelect        = nullptr;      //攻撃選択

    CameraController* cameraController = nullptr;
};