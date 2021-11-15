

#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "result.h"
#include "story.h"
// 追加
#include "gameSystem.h"
#include "audioManager.h"
#include<Windows.h>

extern bool game_exit;
extern int scene_tag;

//////////////////////////////////
void Title::Update(float elapsedTime)
{
	ShowCursor(false);

	GamePad& gamePad = Input::Instance().GetGamePad();
	Mouse& mouseButton = Input::Instance().GetMouse();

	// 決定キー
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


	//	↓	　入力処理とかいろいろ書く　	↓	　//
#if 0
	//startPos.x += 3.0f;
	//if (startPos.x - 450 > 1920)
	//	startPos.x = 450 - 1920;
	//startPos2.x += 3.0f;
	//if (startPos2.x - 450 > 1920)
	//	startPos2.x = 450 - 1920;
	//
	//
	//endPos.x += 3.0f;
	//if (endPos.x- 450 > 1920)
	//	endPos.x = 450 - 1920;
	//endPos2.x += 3.0f;
	//if (endPos2.x - 450 > 1920)
	//	endPos2.x = 450 - 1920;
#endif

	// TODO: タイトル処理
#if 0
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
#endif

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
	C_OffsetBox(StartBox.top, StartBox.left, StartBox.bottom, StartBox.right, 500*0.6f, 280*0.6f );
	//StartBox.left = 0;
	//StartBox.top = 600;
	//C_OffsetBox(StartBox.top, StartBox.left, StartBox.bottom, StartBox.right, 1920, 150 * 0.8f);

	//終了ボックス
	EndBox.left = endPos.x;
	EndBox.top = endPos.y;
	C_OffsetBox(EndBox.top, EndBox.left, EndBox.bottom, EndBox.right, 500 * 0.6f, 280 * 0.6f);

	//EndBox.left = 0;
	//EndBox.top = 800;
	//C_OffsetBox(EndBox.top, EndBox.left, EndBox.bottom, EndBox.right, 1920, 150 * 0.8f);

	//判定 (マウスとゲームへのボックス)
	start_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		StartBox.top, StartBox.left, StartBox.bottom, StartBox.right);

	//判定 (マウスと終了とのボックス)
	end_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		EndBox.top, EndBox.left, EndBox.bottom, EndBox.right);
	
	//当たった時の処理
	if (start_check)
	{
		hit = true;
		framePos.x = startPos.x;
		framePos.y = startPos.y;
		check_state = 1;

		//rgb_A1 = 0.8f;
		//rgb_A2 = 0.6f;
	}
	else if (end_check)
	{
		hit = true;
		framePos.x = endPos.x;
		framePos.y = endPos.y;
		check_state = 2;

		//rgb_A1 = 0.4f;
		//rgb_A2 = 0.9f;

	}
	else
	{
		hit = false;
		check_state = 0;

		//rgb_A1 = 0.4f;
		//rgb_A2 = 0.6f;
	}
	
	switch (check_state)
	{
	case 1:	//ゲームシーンへ
		if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)	ChangeNextScene(new Story());
		break;

	case 2: //ゲーム終了	
		//if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT)	ChangeNextScene(new (Result));
		//if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT) exit(EXIT_SUCCESS);
		if (mouseButton.GetButtonDown() & Mouse::BTN_LEFT) game_exit = true;
		break;
	}
}


void Title::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	
}


void Title::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float title_logoWidth = CAST_F(spr_title_logo->GetTextureWidth());
	float title_logoHeight = CAST_F(spr_title_logo->GetTextureHeight());

	spr_title_logo->Render2(dc,
		0, 0,										// 表示位置
		1.5f, 1.5f,									// スケール
		0, 0,										// 画像切り抜き位置
		title_logoWidth, title_logoHeight,			// 画像切り抜きサイズ
		0, 0,										// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)

	//帯
	//spr_belt1->Render(dc,
	//	0, 600, 1920, 150 * 0.8f,
	//	0, 0, 1920, 150,
	//	0,
	//	1, 1, 1, rgb_A1);
	//spr_belt2->Render(dc,
	//	0, 800, 1920, 150 * 0.8f,
	//	0, 0, 1920, 150,
	//	0,
	//	1, 1, 1, rgb_A2);

	{
		//スタート
		spr_start->Render2(dc,
			startPos.x, startPos.y,
			0.6f, 0.6f,
			0, 0,
			500, 280,
			0, 0,
			(0),
			1, 1, 1, 1

		);

		//終了
		spr_endbox->Render2(dc,
			endPos.x, endPos.y,
			0.6f, 0.6f,
			0, 0,
			500, 280,
			0, 0,
			(0),
			1, 1, 1, 1);

		////スタート
		//spr_start2->Render2(dc,
		//	startPos2.x, startPos2.y,
		//	0.5f, 0.5f,
		//	0, 0,
		//	500, 280,
		//	0, 0,
		//	(0),
		//	1, 1, 1, 1);
		//終了
		//spr_endbox2->Render2(dc,
		//	endPos2.x, endPos2.y,
		//	0.5f, 0.5f,
		//	0, 0,
		//	500, 280,
		//	0, 0,
		//	(0),
		//	1, 1, 1, 1);

		if (hit)
		{
			//確認用フレーム
			spr_frame->Render(dc,
				framePos.x, framePos.y, 400*0.8f, 180*0.8f,
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


void Title::DeInit()
{

}


void Title::Set()
{
	start_check = false;
	end_check = false;

	startPos.x = 450;
	startPos.y = 500;

	//startPos2.x = 450 - 1920;
	//startPos2.y = 600;

	endPos.x = 450;
	endPos.y = 750;

	//endPos2.x = 450 - 1920;
	//endPos2.y = 800;

	framePos.x = 0;
	framePos.y = 0;

	scene_tag = Scene::SCENE_TITLE;

	rgb_A1 = 0.4f;
	rgb_A2 = 0.4f;
}


void Title::Load()
{
	//spr_title_logo		= std::make_unique<Sprite>("Data/Sprite/タイトルロゴ付き背景画像.png");
	spr_title_logo		= std::make_unique<Sprite>("Data/Sprite/title.png");


	spr_mouseCursor		= std::make_unique<Sprite>("Data/Sprite/cursor.png");
	spr_start			= std::make_unique<Sprite>("Data/Sprite/start.png");
	//spr_start			= std::make_unique<Sprite>("Data/Sprite/start.png");
	//spr_start2			= std::make_unique<Sprite>("Data/Sprite/start.png");
	spr_frame			= std::make_unique<Sprite>("Data/Sprite/frame.png");
	spr_endbox			= std::make_unique<Sprite>("Data/Sprite/end.png");
	//spr_endbox			= std::make_unique<Sprite>("Data/Sprite/start.png");
	//spr_endbox2			= std::make_unique<Sprite>("Data/Sprite/start.png");
	spr_belt1			= std::make_unique<Sprite>("Data/Sprite/帯3.png");
	spr_belt2			= std::make_unique<Sprite>("Data/Sprite/帯3.png");
}			

void Title::ImGui()
{
	ImGui::Text("scene : TiTle");

}