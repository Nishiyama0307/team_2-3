#pragma once

#include "timer.h"



// �q�b�g�X�g�b�v ���V���O���g���p�^�[��

class HitStop
{
public:
    HitStop();
    ~HitStop();

public:
    void Update(float& elapsedTime);

public: // Set�֐�
    void SetHitStop() { now_hitstop = true; }

private:
    void HitStopInit();

private:
    bool now_hitstop = false;
    std::unique_ptr<Timer> timer = nullptr;
};