

#include "coutdown.h"



CountDown::CountDown()
{
	now_countdown = true;

    constexpr float start_time_second = 3.0f;

    timer = std::make_unique<Timer>(COUNT::DOWN, start_time_second);
	spr_number = std::make_unique<Sprite>("Data/Sprite/����.png");

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

		// TODO:�X�^�[�g���̒ǉ�
	}
}

void CountDown::SpriteRender(ID3D11DeviceContext* dc, DirectX::XMFLOAT2 pos, DirectX::XMFLOAT2 scale)
{
	if (now_countdown == false) return;


	//�@1�����̕��ƍ������v�Z
	float sw = number_size.x;
	float sh = number_size.y;

	// ���݂̕����ʒu(���Έʒu)
	float carriage = 0;

	// ���l�𕶎��ɕϊ�
	std::string str_number = std::to_string(static_cast<int>(timer->NowTime() + 1.0f));

	DirectX::XMFLOAT4 color = { 1,1,1,1 };

	// �ꕶ���Â�Render()����
	for (const char c : str_number)
	{
		//������\���B�A�X�L�[�R�[�h�̈ʒu�ɂ��镶���ʒu��؂蔲���ĕ\��
		spr_number->Render2(dc, { pos.x + carriage, pos.y }, { scale.x, scale.y }, { sw * (c & 0x0F), sh }, { sw, sh }, { sw * 0.5f, sh * 0.5f }, 0, { color.x, color.y, color.z, color.w });
		//�����ʒu�𕝕����炷
		carriage += number_size.x * scale.x;
	}
}