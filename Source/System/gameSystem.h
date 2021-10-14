#pragma once

#include "Graphics/Graphics.h"
#include <memory>

// 追加
#include "timer.h"
#include "fileIO.h"
#include "hitStop.h"



// ゲームシステム ※シングルトンパターン

class GameSystem
{
private:
    GameSystem() {};
    ~GameSystem() {};

public:
    static GameSystem& Instance()
    {
        static GameSystem instance;
        return instance;
    }

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);

    void Init();
    void DeInit();

    void HitStopUpdate(float& elapsedTime) { hitstop->Update(elapsedTime); }

public: // Get関数
    float NowTime() { return timer->NowTime(); }

public: // Set関数
    void GameStart();
    void StopTime() { timer->Stop(); }
    void UnlockStopTime() { timer->UnlockStop(); }
    void SetHitStop() { hitstop->SetHitStop(); }
    
private: // 変数
    std::unique_ptr<Timer> timer = nullptr;     // 残り時間
    std::unique_ptr<HitStop> hitstop = nullptr; // ヒットストップ
};

// プレイヤーの初期スケールサイズ
constexpr float default_start_value = 3.0f;