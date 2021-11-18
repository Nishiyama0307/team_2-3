////////		このシーンは仮です		/////////////




#include "story.h"
#include "game.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "result.h"
#include "title.h"

// 追加
#include "gameSystem.h"
#include "audioManager.h"
#include<Windows.h>

extern bool is_do_tutorial;

void Story::Update(float elapsedtime)
{

	ShowCursor(false);
	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouseButton = Input::Instance().GetMouse();


	if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)
	{
		story_state++;
	}
	if (story_state > 6)
		story_state = 6;

	//スライドショー
	switch (story_state)
	{
	case 0:
		break;
	case 1:
		posx1 -= 70;
		break;
	case 2:
		posx1 -= 70;
		posx2 -= 70;
		break; 
	case 3:
		posx1 -= 70;
		posx2 -= 70;
		posx3 -= 70;
		break;
	case 4:
		posx1 -= 70;
		posx2 -= 70;
		posx3 -= 70;
		posx4 -= 70;
		break;
	case 5:
		posx1 -= 70;
		posx2 -= 70;
		posx3 -= 70;
		posx4 -= 70;
		posx5 -= 70;
		break;
	case 6:
		posx1 -= 70;
		posx2 -= 70;
		posx3 -= 70;
		posx4 -= 70;
		posx5 -= 70;
		posx6 -= 70;
		is_change = true;
		break;
	}
	
	//マウスボックス
	Mouse& mouse = Input::Instance().GetMouse();
	mousePos.x = mouse.GetPositionX() - 16;
	mousePos.y = mouse.GetPositionY() - 26;
	MouseBox.left = mousePos.x;
	MouseBox.top = mousePos.y;
	C_OffsetBox(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right, 32, 32);

	//スタートのボックス
	StartBox.left = startPos.x;
	StartBox.top = startPos.y;
	C_OffsetBox(StartBox.top, StartBox.left, StartBox.bottom, StartBox.right, 500*0.6, 280*0.6);

	//チュートリアルへボックス
	tutorialBox.left = tutorialPos.x;
	tutorialBox.top =  tutorialPos.y;
	C_OffsetBox(tutorialBox.top, tutorialBox.left, tutorialBox.bottom, tutorialBox.right, 500* 0.6, 280*0.6);

	//判定 (マウスとゲームへのボックス)
	start_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		StartBox.top, StartBox.left, StartBox.bottom, StartBox.right);

	//判定 (マウスとチュートリアルとのボックス)
	tutorial_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		tutorialBox.top, tutorialBox.left, tutorialBox.bottom, tutorialBox.right);

	if (story_state == 6)
	{

		//当たった時の処理
		if (start_check)
		{
			hit = true;
			framePos.x = startPos.x;
			framePos.y = startPos.y;
			check_state = 1;

			timer1++;
			timer2 = 0;
		}
		else if (tutorial_check)
		{
			hit = true;
			framePos.x = tutorialPos.x;
			framePos.y = tutorialPos.y;
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
		case 1:	//ゲームシーンへ
			if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)	ChangeNextScene(new Game());
			is_do_tutorial = false; // チュートリアルなし
			break;

		case 2: //チュートリアルへ
			if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)ChangeNextScene(new Game());
			is_do_tutorial = true; // チュートリアルあり
			break;
		}
	}
}

void Story::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	Graphics& graphics = Graphics::Instance();


}

void Story::SpriteRender(ID3D11DeviceContext* dc)
{

	spr_haikei->Render(dc,
		0,  0, 1920, 1080,
		0, 0, 1920, 1080,
		0,
		1, 1, 1, 1);

#if 0
	{
		//switch (story_state)
		//{
		//case 0:
			spr_story1->Render2(dc,
				0, 0,
				1, 1,
				0, 0,
				1920, 1080,
				0, 0,
				0,
				1, 1, 1, 1);
		//	break;

		//case 1:
			spr_story2->Render2(dc,
				0, 0,
				1, 1,
				0, 0,
				1920, 1080,
				0, 0,
				0,
				1, 1, 1, 1);
		//	break;

		//case 2:
			spr_story3->Render2(dc,
				0, 0,
				1, 1,
				0, 0,
				1920, 1080,
				0, 0,
				0,
				1, 1, 1, 1);
		//	break;

		//case 3:
			spr_story4->Render2(dc,
				0, 0,
				1, 1,
				0, 0,
				1920, 1080,
				0, 0,
				0,
				1, 1, 1, 1);
		//	break;

		//case 4:
			spr_story5->Render2(dc,
				0, 0,
				1, 1,
				0, 0,
				1920, 1080,
				0, 0,
				0,
				1, 1, 1, 1);
		//	break;

		//case 5:
			spr_story6->Render2(dc,
				0, 0,
				1, 1,
				0, 0,
				1920, 1080,
				0, 0,
				0,
				1, 1, 1, 1);
		//	break;
		//case 6:
#endif

			

#if 1
	if (is_change)
	{
		//チュートリアルへ
		{
			//木の板
			spr_wood1->Render2(dc,
				startPos.x, startPos.y,
				0.6f, 0.6f,
				0, 0,
				500, 280,
				0, 0,
				(0),
				1, 1, 1, 1);
			spr_wood2->Render2(dc,
				tutorialPos.x, tutorialPos.y,
				0.6f, 0.6f,
				0, 0,
				500, 280,
				0, 0,
				(0),
				1, 1, 1, 1);
			spr_wood3->Render2(dc,
				500, 130,
				1, 1,
				0, 0,
				900, 280,
				0, 0,
				0,
				1, 1, 1, 1);


			//NO
			if (timer1 / 32 % 2 || !start_check)
				spr_start->Render(dc,
					startPos.x, startPos.y, 500 * 0.6, 280 * 0.6,
					0, 0, 500, 280,
					0,
					1, 1, 1, 1);

			//Yes
			if (timer2 / 32 % 2 || !tutorial_check)
				spr_tutorial->Render(dc,
					tutorialPos.x, tutorialPos.y, 500 * 0.6, 280 * 0.6,
					0, 0, 500, 280,
					0,
					1, 1, 1, 1);

			//question
			spr_question->Render2(dc,
				500, 130,
				1, 1,
				0, 0,
				900, 280,
				0, 0,
				0,
				1, 1, 1, 1);
		}
				//break;
				//}
				if (hit)
				{
					//確認用フレーム
					//spr_frame->Render(dc,
					//	framePos.x, framePos.y, 400, 180,
					//	0, 0, 512, 256,
					//	0,
					//	1, 1, 1, 1);
				}
			}

	//ストーリー
	spr_story6->Render2(dc,
		posx6, 0,
		1, 1,
		0, 0,
		1920, 1080,
		0, 0,
		0,
		1, 1, 1, 1);

	spr_story5->Render2(dc,
		posx5, 0,
		1, 1,
		0, 0,
		1920, 1080,
		0, 0,
		0,
		1, 1, 1, 1);

	spr_story4->Render2(dc,
		posx4, 0,
		1, 1,
		0, 0,
		1920, 1080,
		0, 0,
		0,
		1, 1, 1, 1);

	spr_story3->Render2(dc,
		posx3, 0,
		1, 1,
		0, 0,
		1920, 1080,
		0, 0,
		0,
		1, 1, 1, 1);

	spr_story2->Render2(dc,
		posx2, 0,
		1, 1,
		0, 0,
		1920, 1080,
		0, 0,
		0,
		1, 1, 1, 1);

	spr_story1->Render2(dc,
		posx1, 0,
		1, 1,
		0, 0,
		1920, 1080,
		0, 0,
		0,
		1, 1, 1, 1);

	//マウスカーソル
	spr_mouseCursor->Render(dc,
		mousePos.x, mousePos.y, 64, 64,
		0, 0, 64, 64,
		0,
		1, 1, 1, 1);
#endif
}

void Story::DeInit()
{

}

void Story::Set()
{
	start_check = false;
	tutorial_check = false;

	startPos.x = 1400;
	startPos.y = 800;

	tutorialPos.x = 1400;
	tutorialPos.y = 500;

	framePos.x = 0;
	framePos.y = 0;

	posx1	= 0;
	posx2	= 0;
	posx3	= 0;
	posx4	= 0;
	posx5	= 0;
	posx6	= 0;
	is_change = false;

	//タイトルBGM止める
	AudioManager::Instance().GetAudio(Audio_INDEX::BGM_TITLE)->Stop();
}

void Story::Load()
{
	spr_mouseCursor = std::make_unique<Sprite>("Data/Sprite/cursor.png");
	spr_haikei = std::make_unique<Sprite>("Data/Sprite/story/maou02.png");
	spr_start = std::make_unique<Sprite>("Data/Sprite/select/no.png");
	spr_frame = std::make_unique<Sprite>("Data/Sprite/frame.png");
	spr_tutorial = std::make_unique<Sprite>("Data/Sprite/select/yes.png");

	spr_story1 = std::make_unique<Sprite>("Data/Sprite/story/Story1.png");
	spr_story2 = std::make_unique<Sprite>("Data/Sprite/story/Story2.png");
	spr_story3 = std::make_unique<Sprite>("Data/Sprite/story/Story3.png");
	spr_story4 = std::make_unique<Sprite>("Data/Sprite/story/Story4.png");
	spr_story5 = std::make_unique<Sprite>("Data/Sprite/story/Story5.png");
	spr_story6 = std::make_unique<Sprite>("Data/Sprite/story/Story6.png");

	spr_wood1 = std::make_unique<Sprite>("Data/Sprite/select/wood.png");
	spr_wood2 = std::make_unique<Sprite>("Data/Sprite/select/wood.png");
	spr_wood3 = std::make_unique<Sprite>("Data/Sprite/select/wood3.png");
	spr_question = std::make_unique<Sprite>("Data/Sprite/select/tutorial_what2.png");
}

void Story::ImGui()
{

}