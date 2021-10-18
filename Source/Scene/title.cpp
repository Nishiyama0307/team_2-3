

#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"

// �ǉ�
#include "gameSystem.h"
#include "audioManager.h"

extern bool game_exit;

//////////////////////////////////
void Title::Update(float elapsedTime)
{

	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouseB = Input::Instance().GetMouse();

	// ����L�[
	/*if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
	{
		if (selecting == TUTORIAL)
		{
			ChangeNextScene(new Tutorial());
		}

		if (selecting == GAME)
		{
			ChangeNextScene(new Game());
		}

		if (selecting == START)
		{
			game_mode_select = true;
			selecting = TUTORIAL;
		}

		if (selecting == END) game_exit = true;


		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENTER)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENTER)->Play(false);
	}*/
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)	ChangeNextScene(new Game());




	//	��	�@���͏����Ƃ����낢�돑���@	��	�@//

	// TODO: �^�C�g������
	/*if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	{
		selecting -= 1;

		if (game_mode_select == false)
		{
			if (selecting < 0) selecting = START;
			else select_timer = 0;

		}
		else
		{
			if (selecting < 2) selecting = TUTORIAL;
			else select_timer = 0;
		}


		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);

	}
	if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	{
		selecting += 1;

		if (game_mode_select == false)
		{
			if (selecting > 1) selecting = END;
			else select_timer = 0;
		}
		else
		{
			if (selecting > 3) selecting = GAME;
			else select_timer = 0;
		}

		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);
	}
	if (elapsedTime) select_timer++;*/


	//�}�E�X�{�b�N�X
	Mouse& mouse = Input::Instance().GetMouse();
	mousePos.x = mouse.GetPositionX() - 32;
	mousePos.y = mouse.GetPositionY() - 32;
	MouseBox.left = mousePos.x;
	MouseBox.top = mousePos.y;
	C_OffsetBox(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right, 64, 64);

	//�X�^�[�g�̃{�b�N�X
	StartBox.left = StartPos.x;
	StartBox.top = StartPos.y;
	C_OffsetBox(StartBox.top, StartBox.left, StartBox.bottom, StartBox.right, 512, 256);


#if false
	//�����̂̃{�b�N�X
	NoneBox.size.x = 512;
	NoneBox.size.y = 256;
	NoneBox.left = 300;
	NoneBox.top = 1000;
	NoneBox.right = NoneBox.left + NoneBox.size.x;
	NoneBox.bottom = NoneBox.top + NoneBox.size.y;
#endif
	//����
	check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		StartBox.top, StartBox.left, StartBox.bottom, StartBox.right);

	if (check)
	{
		hit = true;
	}
	else
	{
		hit = false;
	}

}


void Title::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3D���f���̕`�� */
	
}


void Title::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2D�X�v���C�g�̕`�� */
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float title_logoWidth = CAST_F(spr_title_logo->GetTextureWidth());
	float title_logoHeight = CAST_F(spr_title_logo->GetTextureHeight());
	float spr_playWidth = CAST_F(spr_play->GetTextureWidth());
	float spr_playHeight = CAST_F(spr_play->GetTextureHeight());
	float spr_endWidth = CAST_F(spr_end->GetTextureWidth());
	float spr_endHeight = CAST_F(spr_end->GetTextureHeight());

	spr_title_logo->Render2(dc,
		0, 0,						// �\���ʒu
		1.0f, 1.0f,									// �X�P�[��
		0, 0,										// �摜�؂蔲���ʒu
		title_logoWidth, title_logoHeight,				// �摜�؂蔲���T�C�Y
		0, 0,	// �摜��_
		angle,										// �p�x
		1, 1, 1, 1);								// �F���(r,g,b,a)

	spr_space->Render2(dc,
		0, 0,						// �\���ʒu
		1.0f, 1.0f,									// �X�P�[��
		0, 0,										// �摜�؂蔲���ʒu
		title_logoWidth, title_logoHeight,				// �摜�؂蔲���T�C�Y
		0, 0,	// �摜��_
		angle,										// �p�x
		1, 1, 1, 1);								// �F���(r,g,b,a)



	spr_play->Render2(dc,
		0, 0,						// �\���ʒu
		1.0f, 1.0f,									// �X�P�[��
		0, 0,										// �摜�؂蔲���ʒu
		spr_playWidth, spr_playHeight,				// �摜�؂蔲���T�C�Y
		0, 0,	// �摜��_
		angle,										// �p�x
		1, 1, 1, 1);								// �F���(r,g,b,a)

	spr_end->Render2(dc,
		0, 0,						// �\���ʒu
		1.0f, 1.0f,									// �X�P�[��
		0, 0,										// �摜�؂蔲���ʒu
		spr_endWidth, spr_endHeight,				// �摜�؂蔲���T�C�Y
		0, 0,	// �摜��_
		angle,										// �p�x
		1, 1, 1, 1);								// �F���(r,g,b,a)
	
	{
		//�X�^�[�g
		start->Render(dc,
			StartPos.x, StartPos.y, 512, 256,
			0, 0, 512, 256,
			0,
			1, 1, 1, 1);

		if (hit)
		{
			frame->Render(dc,
				300, 700, 512, 256,
				0, 0, 512, 256,
				0,
				1, 1, 1, 1);
		}

		//�}�E�X�J�[�\��
		mouseCursor->Render(dc,
			mousePos.x, mousePos.y, 64, 64,
			0, 0, 64, 64,
			0,
			1, 1, 1, 1);
	}
}


void Title::DeInit()
{
	safe_del(mouseCursor);
	safe_del(start);
	safe_del(frame);
}


void Title::Set()
{
	check = false;

	StartPos.x = 300;
	StartPos.y = 700;
}


void Title::Load()
{
	spr_title_logo		= std::make_unique<Sprite>("Data/Sprite/�^�C�g�����S�t���w�i�摜.png");
	spr_space			= std::make_unique<Sprite>("Data/Sprite/space�i�^�C�g���j.png");
	spr_play			= std::make_unique<Sprite>("Data/Sprite/�X�^�[�g�i�^�C�g���j.png");
	spr_end 			= std::make_unique<Sprite>("Data/Sprite/��߂�i�^�C�g���j.png");


	mouseCursor = new Sprite("Data/Sprite/cursor.png");
	start = new Sprite("Data/Sprite/�X�^�[�g.png");
	frame = new Sprite("Data/Sprite/frame.png");
}


void Title::ImGui()
{
	ImGui::Text("scene : TiTle");

}