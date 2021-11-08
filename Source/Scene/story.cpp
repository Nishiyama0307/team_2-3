////////		このシーンは仮です		/////////////




#include "story.h"
#include "tutorial.h"
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

void Story::Update(float elapsedtime)
{
	ShowCursor(false);
	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouseButton = Input::Instance().GetMouse();

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
	C_OffsetBox(StartBox.top, StartBox.left, StartBox.bottom, StartBox.right, 400, 180);

	//チュートリアルへボックス
	tutorialBox.left = tutorialPos.x;
	tutorialBox.top =  tutorialPos.y;
	C_OffsetBox(tutorialBox.top, tutorialBox.left, tutorialBox.bottom, tutorialBox.right, 400, 180);

	//判定 (マウスとゲームへのボックス)
	start_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		StartBox.top, StartBox.left, StartBox.bottom, StartBox.right);

	//判定 (マウスとチュートリアルとのボックス)
	tutorial_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		tutorialBox.top, tutorialBox.left, tutorialBox.bottom, tutorialBox.right);

	//当たった時の処理
	if (start_check)
	{
		hit = true;
		framePos.x = startPos.x;
		framePos.y = startPos.y;
		check_state = 1;
	}
	else if (tutorial_check)
	{
		hit = true;
		framePos.x = tutorialPos.x;
		framePos.y = tutorialPos.y;
		check_state = 2;
	}
	else
	{
		hit = false;
		check_state = 0;
	}

	switch (check_state)
	{
	case 1:	//ゲームシーンへ
		if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)	ChangeNextScene(new Game());
		break;

	case 2: //チュートリアルへ
		if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)ChangeNextScene(new Tutorial());
		break;
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

	{
		//NO
		spr_start->Render(dc,
			startPos.x, startPos.y, 400, 180,
			0, 0, 400, 180,
			0,
			1, 1, 1, 1);

		//Yes
		spr_tutorial->Render(dc,
			tutorialPos.x, tutorialPos.y, 400, 180,
			0, 0, 400, 180,
			0,
			1, 1, 1, 1);

		if (hit)
		{
			//確認用フレーム
			spr_frame->Render(dc,
				framePos.x, framePos.y, 400, 180,
				0, 0, 512, 256,
				0,
				1, 1, 1, 1);
		}

		//マウスカーソル
		spr_mouseCursor->Render(dc,
			mousePos.x, mousePos.y, 64, 64,
			0, 0, 64, 64,
			0,
			1, 1, 1, 1);
	}
}

void Story::DeInit()
{

}

void Story::Set()
{
	start_check = false;
	tutorial_check = false;

	startPos.x = 300;
	startPos.y = 700;

	tutorialPos.x = 1000;
	tutorialPos.y = 700;

	framePos.x = 0;
	framePos.y = 0;
}

void Story::Load()
{
	spr_mouseCursor = std::make_unique<Sprite>("Data/Sprite/cursor.png");
	spr_haikei = std::make_unique<Sprite>("Data/Sprite/ストーリー.png");
	spr_start = std::make_unique<Sprite>("Data/Sprite/no.png");
	spr_frame = std::make_unique<Sprite>("Data/Sprite/frame.png");
	spr_tutorial = std::make_unique<Sprite>("Data/Sprite/yes.png");
}

void Story::ImGui()
{

}