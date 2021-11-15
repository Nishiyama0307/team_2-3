#pragma once

#include "Graphics/Sprite.h"
#include <memory>
#include "digits.h"

enum class COUNT
{
    UP,
    DOWN,
};



// タイマー

class Timer : public Digits<float>
{
public:
    Timer(COUNT count,const float default_start_time_, const bool render = false);
    ~Timer() {};

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos = {}, DirectX::XMFLOAT2 scale = { 1,1 }, DirectX::XMFLOAT4 color = { 1,1,1,1 });

public: // Get関数
    float NowTime() { return GetDigit(); }

public: // Set関数
    void Reset() { Digits::Reset(); }
    void Add(const float add) { Digits::Add(add); }
    void Subtract(const float subtract) { Digits::Subtract(subtract); }
    void Stop() { stop = true; }
    void UnlockStop() { stop = false; }

private:
    //　関数ポインタ //
    using pFunc = void(Timer::*)(float);
    pFunc pCount = nullptr;

    // 関数ポインタ用関数
    void CountUp(float elapsedTime);
    void CountDown(float elapsedTime);

private: // 定数
    static constexpr float zero_time = 0.0f;        // ゼロ
    static constexpr float max_time = 9999.0f;      // 最大値

private: // 変数
    std::unique_ptr<Sprite> spr_number = nullptr;   // スプライト
    DirectX::XMFLOAT2 number_size = {};             // 一文字の大きさ
    bool stop = false;                              // 一時停止
};