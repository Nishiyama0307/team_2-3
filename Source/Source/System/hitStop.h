#pragma once

#include "timer.h"



// ヒットストップ ※シングルトンパターン

class HitStop
{
public:
    HitStop();
    ~HitStop();

public:
    void Update(float& elapsedTime);

public: // Set関数
    void SetHitStop() { now_hitstop = true; }

private:
    void HitStopInit();

private:
    bool now_hitstop = false;
    std::unique_ptr<Timer> timer = nullptr;
};