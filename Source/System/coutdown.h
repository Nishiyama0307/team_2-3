#pragma once

#include "timer.h"
#include "Graphics/Sprite.h"



// カウントダウンクラス

class CountDown
{
public:
    CountDown();
    ~CountDown();

public:
    void Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos = {}, DirectX::XMFLOAT2 scale = { 1,1 });

public: // Get関数
    bool NowCountDown() { return now_countdown; }

private:
    bool now_countdown = true;
    std::unique_ptr<Timer> timer = nullptr;
    std::unique_ptr<Sprite> spr_number = nullptr;
    DirectX::XMFLOAT2 number_size = {};
};