

#include "gameSystem.h"
#include <algorithm>



void GameSystem::Update(float elapsedTime)
{
    timer->Update(elapsedTime);
}


void GameSystem::SpriteRender(ID3D11DeviceContext* dc)
{
    DirectX::XMFLOAT4 color = { 1,1,1,1 };

    if (timer->NowTime() <= 30.0f)
    {
        color = { 1,1,0,1 };
    }

    if (timer->NowTime() <= 10.0f)
    {
        color = { 1,0,0,1 };
    }



    timer->SpriteRender(dc, { 850,0 }, { 1,1 }, color);
    //score->SpriteRender(dc);
}

void GameSystem::Init()
{
    //constexpr float time_limit_second = 80.8f;
    constexpr float time_limit_second = 5.0f;

    timer = std::make_unique<Timer>(COUNT::DOWN, time_limit_second, true);
    hitstop = std::make_unique<HitStop>();

}


void GameSystem::DeInit()
{
    
}


void GameSystem::GameStart()
{
    timer->Reset();
#if _DEBUG
    timer->Stop();
#endif
}
