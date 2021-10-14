#pragma once

#include "Graphics/Graphics.h"
#include <memory>

// �ǉ�
#include "timer.h"
#include "fileIO.h"
#include "hitStop.h"



// �Q�[���V�X�e�� ���V���O���g���p�^�[��

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

public: // Get�֐�
    float NowTime() { return timer->NowTime(); }

public: // Set�֐�
    void GameStart();
    void StopTime() { timer->Stop(); }
    void UnlockStopTime() { timer->UnlockStop(); }
    void SetHitStop() { hitstop->SetHitStop(); }
    
private: // �ϐ�
    std::unique_ptr<Timer> timer = nullptr;     // �c�莞��
    std::unique_ptr<HitStop> hitstop = nullptr; // �q�b�g�X�g�b�v
};

// �v���C���[�̏����X�P�[���T�C�Y
constexpr float default_start_value = 3.0f;