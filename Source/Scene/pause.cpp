

#include "pause.h"
#include "common.h"
#include "Input/Input.h"

#include "title.h"
#include "audioManager.h"

#include "Input/Mouse.h"
#include "scene.h"
#include "title.h"

bool f1; //�|�[�Y�������A�U���ƃ|�[�Y�����̍��N���b�N�������ɏ��������̂łP�t���[�������������Ȃ�
#include<Windows.h>

Pause::Pause(Scene* scene_) : scene(scene_)
{
	now_pause = false;

	spr_mouseCursor = std::make_unique<Sprite>("Data/Sprite/cursor.png");
	spr_start = std::make_unique<Sprite>("Data/Sprite/select/gamebuck.png");
	spr_endbox = std::make_unique<Sprite>("Data/Sprite/select/titlebuck.png");
	spr_back2 = std::make_unique<Sprite>("Data/Sprite/back.png");

	spr_wood1 = std::make_unique<Sprite>("Data/Sprite/select/wood.png");
	spr_wood2 = std::make_unique<Sprite>("Data/Sprite/select/wood.png");

	start_check = false;
	end_check = false;

	startPos.x = 300;
	startPos.y = 700;

	endPos.x = 1000;
	endPos.y = 700;

	framePos.x = 0;
	framePos.y = 0;

	
}


Pause::~Pause()
{

}


bool Pause::Update(float elapsedTime)
{
#if false
	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_R)
	{
		now_pause = !now_pause;

		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);
	}

	if (now_pause == true)
	{
		GamePad& gamePad = Input::Instance().GetGamePad();

		if (selecting == CONTINUE)
			if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE)
			{
				now_pause = false;

				AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENTER)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENTER)->Play(false);
			}


		if (selecting == END)
			if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE)
			{
				scene->ChangeNextScene(new Title());
				if (scene->bgm_caution == false)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
				if (scene->bgm_caution == true)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_SPEED)->Stop();
			}

		if (gamePad.GetButtonDown() & GamePad::BTN_UP)
		{
			selecting -= 1;

			if (selecting < 0) selecting = CONTINUE;
			else select_timer = 0;

			AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
			AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);
		}
		if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
		{
			selecting += 1;

			if (selecting > 1) selecting = END;
			else select_timer = 0;

			AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
			AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);
		}
		if (elapsedTime) select_timer++;
	}
#endif

	ShowCursor(false);
#ifdef _DEBUG
	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_R)
#else
	if(Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_ESC)
#endif
	{
		now_pause = !now_pause;

		//AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
		//AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);
	}

	
	Mouse& mouseButton = Input::Instance().GetMouse();
	//�}�E�X�{�b�N�X
	Mouse& mouse = Input::Instance().GetMouse();
	mousePos.x = mouse.GetPositionX() - 16;
	mousePos.y = mouse.GetPositionY() - 26;
	MouseBox.left = mousePos.x;
	MouseBox.top = mousePos.y;
	C_OffsetBox(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right, 32, 32);

	//�Q�[�����s�̃{�b�N�X
	StartBox.left = startPos.x;
	StartBox.top = startPos.y;
	C_OffsetBox(StartBox.top, StartBox.left, StartBox.bottom, StartBox.right, 500, 280);

	//�^�C�g���փ{�b�N�X
	EndBox.left = endPos.x;
	EndBox.top = endPos.y;
	C_OffsetBox(EndBox.top, EndBox.left, EndBox.bottom, EndBox.right, 500, 280);

	//���� (�}�E�X�ƃQ�[�����s�ւ̃{�b�N�X)
	start_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		StartBox.top, StartBox.left, StartBox.bottom, StartBox.right);

	//���� (�}�E�X�ƃ^�C�g���ւƂ̃{�b�N�X)
	end_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		EndBox.top, EndBox.left, EndBox.bottom, EndBox.right);
	if (now_pause == true)
	{
		//�����������̏���
		if (start_check)
		{
			hit = true;
			framePos.x = startPos.x;
			framePos.y = startPos.y;
			check_state = 1;

			timer1++;
			timer2 = 0;
		}
		else if (end_check)
		{
			hit = true;
			framePos.x = endPos.x;
			framePos.y = endPos.y;
			check_state = 2;

			timer1 = 0;
			timer2++;
		}
		else
		{
			hit = false;
			check_state = 0;

			timer1 = 0;
			timer2 = 0;
		}

		switch (check_state)
		{
		case 1:	//�Q�[�����s
			if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)
			{
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_CLICK)->Play(false);
				now_pause = false;
			}
			f1 = false;
			break;

		case 2: //�^�C�g����
			if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)
			{
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_CLICK)->Play(false);
				scene->ChangeNextScene(new Title());
			}
			break;
		}
	}
	return now_pause;
}


void Pause::SpriteRender(ID3D11DeviceContext* dc)
{
#if _RELEASE
	spr_ui->Render2(dc, { 0,0 }, { 1,1 },
		{ 0,0 },
		{ 1920,1080 },
		{ 0,0 },
		0,
		{ 1,1,1,1 }
	);
#endif

	if (now_pause == false) return;

	/* 2D�X�v���C�g�̕`�� */

	{
		//�w��
		spr_back2->Render(dc,
			0, 0, 1920, 1080,
			0, 0, 1920, 1080,
			0,
			0, 0, 0, 0.6f);

		spr_wood1->Render2(dc,
			startPos.x, startPos.y,
			1.0f, 1.0f,
			0, 0,
			500, 280,
			0, 0,
			(0),
			1, 1, 1, 1);
		spr_wood2->Render2(dc,
			endPos.x, endPos.y,
			1.0f, 1.0f,
			0, 0,
			500, 280,
			0, 0,
			(0),
			1, 1, 1, 1);

		//���s
		if(timer1 / 32 % 2 || !start_check)
		spr_start->Render(dc,
			startPos.x, startPos.y, 500, 280,
			0, 0, 500, 280,
			0,
			1, 1, 1, 1);

		//�^�C�g����
		if(timer2 / 32 % 2 || !end_check)
		spr_endbox->Render(dc,
			endPos.x, endPos.y, 500 , 280 ,
			0, 0, 500, 280,
			0,
			1, 1, 1, 1);

		if (hit)
		{
			//�m�F�p�t���[��
			//spr_frame->Render(dc,
			//	framePos.x, framePos.y, 400, 180,
			//	0, 0, 512, 256,
			//	0,
			//	1, 1, 1, 1);
		}

		//�}�E�X�J�[�\��
		spr_mouseCursor->Render(dc,
			mousePos.x, mousePos.y, 64, 64,
			0, 0, 64, 64,
			0,
			1, 1, 1, 1);
	}

	

	//spr_back->Render2(dc,
	//	0, 0,						// �\���ʒu
	//	1.0f, 1.0f,									// �X�P�[��
	//	0, 0,										// �摜�؂蔲���ʒu
	//	spr_playWidth, spr_playHeight,				// �摜�؂蔲���T�C�Y
	//	0, 0,	// �摜��_
	//	angle,										// �p�x
	//	1, 1, 1, 1);								// �F���(r,g,b,a)
	//
	//if (selecting == CONTINUE)
	//{
	//	if (select_timer >> 5 & 0x01)
	//	{
	//		spr_play->Render2(dc,
	//			0,0,						// �\���ʒu
	//			1.0f, 1.0f,									// �X�P�[��
	//			0, 0,										// �摜�؂蔲���ʒu
	//			spr_playWidth, spr_playHeight,				// �摜�؂蔲���T�C�Y
	//			0, 0,	// �摜��_
	//			angle,										// �p�x
	//			1, 1, 1, 1);								// �F���(r,g,b,a)
	//	}
	//}
	//else
	//{
	//	spr_play->Render2(dc,
	//		0, 0,						// �\���ʒu
	//		1.0f, 1.0f,									// �X�P�[��
	//		0, 0,										// �摜�؂蔲���ʒu
	//		spr_playWidth, spr_playHeight,				// �摜�؂蔲���T�C�Y
	//		0, 0,	// �摜��_
	//		angle,										// �p�x
	//		1, 1, 1, 1);								// �F���(r,g,b,a)
	//}
	//
	//if (selecting == END)
	//{
	//	if (select_timer >> 5 & 0x01)
	//	{
	//		spr_end->Render2(dc,
	//			0, 0,						// �\���ʒu
	//			1.0f, 1.0f,									// �X�P�[��
	//			0, 0,										// �摜�؂蔲���ʒu
	//			spr_endWidth, spr_endHeight,				// �摜�؂蔲���T�C�Y
	//			0, 0,	// �摜��_
	//			angle,										// �p�x
	//			1, 1, 1, 1);								// �F���(r,g,b,a)
	//	}
	//}
	//else
	//{
	//	spr_end->Render2(dc,
	//		0, 0,						// �\���ʒu
	//		1.0f, 1.0f,									// �X�P�[��
	//		0, 0,										// �摜�؂蔲���ʒu
	//		spr_endWidth, spr_endHeight,				// �摜�؂蔲���T�C�Y
	//		0, 0,	// �摜��_
	//		angle,										// �p�x
	//		1, 1, 1, 1);								// �F���(r,g,b,a)
	//}
}