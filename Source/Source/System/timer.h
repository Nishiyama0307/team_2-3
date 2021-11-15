#pragma once

#include "Graphics/Sprite.h"
#include <memory>
#include "digits.h"

enum class COUNT
{
    UP,
    DOWN,
};



// �^�C�}�[

class Timer : public Digits<float>
{
public:
    Timer(COUNT count,const float default_start_time_, const bool render = false);
    ~Timer() {};

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos = {}, DirectX::XMFLOAT2 scale = { 1,1 }, DirectX::XMFLOAT4 color = { 1,1,1,1 });

public: // Get�֐�
    float NowTime() { return GetDigit(); }

public: // Set�֐�
    void Reset() { Digits::Reset(); }
    void Add(const float add) { Digits::Add(add); }
    void Subtract(const float subtract) { Digits::Subtract(subtract); }
    void Stop() { stop = true; }
    void UnlockStop() { stop = false; }

private:
    //�@�֐��|�C���^ //
    using pFunc = void(Timer::*)(float);
    pFunc pCount = nullptr;

    // �֐��|�C���^�p�֐�
    void CountUp(float elapsedTime);
    void CountDown(float elapsedTime);

private: // �萔
    static constexpr float zero_time = 0.0f;        // �[��
    static constexpr float max_time = 9999.0f;      // �ő�l

private: // �ϐ�
    std::unique_ptr<Sprite> spr_number = nullptr;   // �X�v���C�g
    DirectX::XMFLOAT2 number_size = {};             // �ꕶ���̑傫��
    bool stop = false;                              // �ꎞ��~
};