

#include "timer.h"
#include "common.h"



Timer::Timer(COUNT count, const float default_start_time_, const bool render) : Digits(default_start_time_, 2)
{
    stop = false;

    // 三項演算子
    pCount = static_cast<int>(count) ? /* DOWN = true */&Timer::CountDown : /* Up = false */&Timer::CountUp;


    // 描画する場合のみスプライトの実体を生成
    if (render == false) return;

    spr_number = std::make_unique<Sprite>("Data/Sprite/数字.png");

    number_size.x = static_cast<float>(spr_number->GetTextureWidth() / 10);
    number_size.y = static_cast<float>(spr_number->GetTextureHeight());

}


void Timer::Update(float elapsedTime)
{
    if (stop) return;

    // 時間の更新
    (this->*pCount)(elapsedTime);
}


void Timer::SpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT4 color)
{
    if (spr_number == nullptr) return;


    /* 2Dスプライトの描画 */

    std::vector<float> digit_place;
    digit_place.resize(max_digits_place);
    SliceDigits(digit, digit_place);

    for (size_t i = digit_place.size(); i > 0; --i)
    {
        spr_number->Render2(dc,
            { pos.x + number_size.x * (i - 1),pos.y },
            { scale.x,scale.y },
            { 0 + number_size.x * digit_place.at(digit_place.size() - i) ,0 }, { number_size.x, number_size.y },
            { 0,0 },
            0,
            color);
    }
}


void Timer::CountUp(float elapsedTime)
{
    // タイマーが限界値になったら更新しない
    if (digit == max_time) return;


    digit += elapsedTime;


    //TODO: 限界値 999.0f　まで
    // 限界地を超えたら
    if (digit < max_time) return;
    digit = max_time;

}


void Timer::CountDown(float elapsedTime)
{
    // タイマーが０になったら更新しない
    if (digit == zero_time) return;


    digit += -elapsedTime;


    // ゼロより下回ったら
    if (digit > zero_time) return;
    digit = zero_time;
}