

#include "coutdown.h"



CountDown::CountDown()
{
	now_countdown = true;

    constexpr float start_time_second = 3.0f;

    timer = std::make_unique<Timer>(COUNT::DOWN, start_time_second);
	spr_number = std::make_unique<Sprite>("Data/Sprite/数字.png");

	number_size.x = static_cast<float>(spr_number->GetTextureWidth() / 10);
	number_size.y = static_cast<float>(spr_number->GetTextureHeight());
}

CountDown::~CountDown()
{

}

void CountDown::Update(float elapsedTime)
{
	if (now_countdown == false) return;

    timer->Update(elapsedTime);
    
    if(timer->NowTime() <= 0)
	{
		now_countdown = false;

		// TODO:スタート音の追加
	}
}

void CountDown::SpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale)
{
	if (now_countdown == false) return;


	//　1文字の幅と高さを計算
	float sw = number_size.x;
	float sh = number_size.y;

	// 現在の文字位置(相対位置)
	float carriage = 0;

	// 数値を文字に変換
	std::string str_number = std::to_string(static_cast<int>(timer->NowTime() + 1.0f));

	DirectX::XMFLOAT4 color = { 1,1,1,1 };

	// 一文字づつRender()する
	for (const char c : str_number)
	{
		//文字を表示。アスキーコードの位置にある文字位置を切り抜いて表示
		spr_number->Render2(dc, { pos.x + carriage, pos.y }, { scale.x, scale.y }, { sw * (c & 0x0F), sh }, { sw, sh }, { sw * 0.5f, sh * 0.5f }, 0, { color.x, color.y, color.z, color.w });
		//文字位置を幅分ずらす
		carriage += number_size.x * scale.x;
	}
}