

#include "hitStop.h"



HitStop::HitStop()
{
    float during_stop_time_second = 0.025f;

    timer = std::make_unique<Timer>(COUNT::DOWN, during_stop_time_second);

    now_hitstop = false;
}

HitStop::~HitStop()
{

}


void HitStop::Update(float& elapsedTime)
{
    if (now_hitstop == false) return;


    if (timer->NowTime() <= 0.0f) { HitStopInit(); return; }


    timer->Update(elapsedTime);

    elapsedTime *= 0.3f;

}


void HitStop::HitStopInit()
{
    now_hitstop = false;
    timer->Reset();
}