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
#include "pause.h"
#include "coutdown.h"
#include "C_function.h"


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

    //UI達
    Sprite* HpBar = nullptr;				//HP
    Sprite* HpBarFrame = nullptr;			//
    Sprite* StressBar = nullptr;			//ストレス
    Sprite* StressBarFrame = nullptr;		//
    Sprite* GoldBar = nullptr;				//お金
    Sprite* Minimap = nullptr;				//ミニマップ
    Sprite* Castlebar = nullptr;			//城
    Sprite* CastlebarFrame = nullptr;		//
    Sprite* AttackSlot = nullptr;			//攻撃スロット
};