

#include "result.h"
#include "game.h"
#include "title.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"

// 追加
#include "gameSystem.h"
#include "audioManager.h"

int result;

void Result::Update(float elapsedTime)
{
	Mouse& mouseButton = Input::Instance().GetMouse();
	// シーン変更
	{
		ChangeScene(elapsedTime);
	}


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: 結果処理
	GamePad& gamePad = Input::Instance().GetGamePad();

	//マウス
	Mouse& mouse = Input::Instance().GetMouse();
	mousePos.x = mouse.GetPositionX() - 16;
	mousePos.y = mouse.GetPositionY() - 26;
	MouseBox.left = mousePos.x;
	MouseBox.top = mousePos.y;
	C_OffsetBox(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right, 32, 32);

	//スタートのボックス
	backBox.left = backPos.x;
	backBox.top = backPos.y;
	C_OffsetBox(backBox.top, backBox.left, backBox.bottom, backBox.right, 500 * 0.6f, 280 * 0.6f);

	//判定 (マウスとゲームへのボックス)
	back_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
		backBox.top, backBox.left, backBox.bottom, backBox.right);

	//当たった時の処理
	if (back_check)
	{
		hit = true;
		timer1++;
	}
	else
	{
		timer1 = 0;
		hit = false;
	}

	if (hit && mouseButton.GetButtonDown() & Mouse::BTN_LEFT)	
	{
		ChangeNextScene(new Title());
	}

	//if (gamePad.GetButtonDown() & GamePad::BTN_UP)
	//{
	//	AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
	//	AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);
	//
	//	selecting -= 1;  
	//
	//	if (selecting < 0) selecting = RETRY;
	//	else select_timer = 0;
	//}
	//if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)
	//{
	//	AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Stop();
	//	AudioManager::Instance().GetAudio(Audio_INDEX::SE_SELECT)->Play(false);
	//
	//	selecting += 1;
	//
	//	if (selecting > 1) selecting = END;
	//	else select_timer = 0;
	//}
	//if (elapsedTime) select_timer++;


	switch (result)
	{
		//クリア
	case Game_clear:
		backPos.x = 300;
		backPos.y = 650;
		break;

		//自身の体力が0
	case Game_over1:
		backPos.x = 700;
		backPos.y = 850;
		break;

		//城の体力が0
	case Game_over2:
		backPos.x = 300;
		backPos.y = 350;
		break;
	}



	// 黒帯の時間更新
	if (did_first == false)
	{
		black_band_timer += 1.0f * elapsedTime;

		if (ClampMax(black_band_timer, black_band_timer_max))
		{
			did_first = true;
			black_band_timer = 0.0f;

		}
	}

}


void Result::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{

}


void Result::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float spr_retryWidth = CAST_F(spr_retry->GetTextureWidth());
	float spr_retryHeight = CAST_F(spr_retry->GetTextureHeight());
	float spr_endWidth = CAST_F(spr_end->GetTextureWidth());
	float spr_endHeight = CAST_F(spr_end->GetTextureHeight());

#if 0

	spr_back->Render2(dc,
		0, 0,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		screenWidth, screenHeight,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)

	if (selecting == RETRY)
	{
		if (select_timer >> 5 & 0x01)
		{
			spr_retry->Render2(dc,
				0, 0,						// 表示位置
				1.0f, 1.0f,									// スケール
				0, 0,										// 画像切り抜き位置
				spr_retryWidth, spr_retryHeight,				// 画像切り抜きサイズ
				0, 0,	// 画像基準点
				angle,										// 角度
				1, 1, 1, 1);								// 色情報(r,g,b,a)
		}
	}
	else
	{
		spr_retry->Render2(dc,
			0, 0,						// 表示位置
			1.0f, 1.0f,									// スケール
			0, 0,										// 画像切り抜き位置
			spr_retryWidth, spr_retryHeight,				// 画像切り抜きサイズ
			0, 0,	// 画像基準点
			angle,										// 角度
			1, 1, 1, 1);								// 色情報(r,g,b,a)
	}

	if (selecting == END)
	{
		if (select_timer >> 5 & 0x01)
		{
			spr_end->Render2(dc,
				0, 0,						// 表示位置
				1.0f, 1.0f,									// スケール
				0, 0,										// 画像切り抜き位置
				spr_endWidth, spr_endHeight,				// 画像切り抜きサイズ
				0, 0,	// 画像基準点
				angle,										// 角度
				1, 1, 1, 1);								// 色情報(r,g,b,a)
		}
	}
	else
	{
		spr_end->Render2(dc,
			0, 0,						// 表示位置
			1.0f, 1.0f,									// スケール
			0, 0,										// 画像切り抜き位置
			spr_endWidth, spr_endHeight,				// 画像切り抜きサイズ
			0, 0,	// 画像基準点
			angle,										// 角度
			1, 1, 1, 1);								// 色情報(r,g,b,a)
	}
# endif

	//分岐
	switch (result)
	{
		//クリア
	case Game_clear:
		spr_clear->Render2(dc,
			0, 0,
			1.0f, 1.0f,
			0, 0,
			1920, 1080,
			0, 0,
			0,
			1, 1, 1, 1);
		break;

		//自身の体力が0
	case Game_over1:
		spr_over1->Render2(dc,
			0, 0,
			1.5f, 1.5f,
			0, 0,
			1280, 720,
			0, 0,
			angle,
			1, 1, 1, 1);
		break;

		//城の体力が0
	case Game_over2:
		spr_over2->Render2(dc,
			0, 0,
			1.5f, 1.5f,
			0, 0,
			1280, 720,
			0, 0,
			angle,
			1, 1, 1, 1);
		break;
	}


	//看板みたいなやつ
	spr_wood1->Render2(dc,
		backPos.x, backPos.y,
		0.6f, 0.6f,
		0, 0,
		500, 280,
		0, 0,
		(0),
		1, 1, 1, 1);

	//タイトルへ戻る
	if (timer1 / 32 % 2 || !back_check)
	spr_back2->Render2(dc,
		backPos.x, backPos.y,
		0.6f, 0.6f,
		0, 0,
		500, 280,
		0, 0,
		(0),
		1, 1, 1, 1);

	//マウスカーソル
	spr_mouseCursor->Render(dc,
		mousePos.x, mousePos.y, 64, 64,
		0, 0, 64, 64,
		0,
		1, 1, 1, 1);


	// 黒帯
	constexpr float scale = 300.0f;

	if(did_first == false)black_band->Render(dc, 0, 0, 1920, 540 - scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
	if(did_first == false)black_band->Render(dc, 0, 540 + scale * pow(black_band_timer, 5), 1920, 540, 0, 0, 0, 0, 0, 1, 1, 1, 1);

	if(did_first)ChangeSceneSpriteRender(dc);

}


void Result::DeInit()
{

}


void Result::Set()
{
	black_band_timer = 0.0f;
	did_first = false;
	did = false;

	backPos.x = 700;
	backPos.y = 850;

	AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE4)->Stop();
	AudioManager::Instance().GetAudio(Audio_INDEX::SE_RESULT)->Play(false);
}


void Result::Load()
{
	spr_back = std::make_unique<Sprite>("Data/Sprite/タイトルロゴなし背景画像（リザルト）.png");
	spr_retry = std::make_unique<Sprite>("Data/Sprite/リトライ（リザルト）.png");
	spr_end = std::make_unique<Sprite>("Data/Sprite/やめる（リザルト）.png");
	spr_font = std::make_unique<Sprite>("Data/Sprite/数字.png");


	black_band = std::make_unique<Sprite>();

	spr_mouseCursor = std::make_unique<Sprite>("Data/Sprite/cursor.png");

	spr_clear = std::make_unique<Sprite>("Data/Sprite/scene/クリア.png");
	spr_over1 = std::make_unique<Sprite>("Data/Sprite/scene/maou_down.png");
	spr_over2 = std::make_unique<Sprite>("Data/Sprite/scene/maou_houkai.png");

	spr_wood1 = std::make_unique<Sprite>("Data/Sprite/select/wood.png");
	spr_back2 = std::make_unique<Sprite>("Data/Sprite/select/titlebuck.png");

}


void Result::ImGui()
{
	ImGui::Text("scene : Result");
}


void Result::ChangeScene(float elapsedTime)
{
	if (Input::Instance().GetGamePad().GetButtonDown() & GamePad::BTN_SPACE)
	{
		did = true;
	}

	if (did == false) return;

	// 黒帯の更新
	black_band_timer += 1.0f * elapsedTime;


	// 黒帯が降りきったら
	if (black_band_timer >= 1.4f)
	{
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_RESULT)->Stop();

		if (selecting == RETRY)
			ChangeNextScene(new Game(), true);

		if (selecting == END)
			ChangeNextScene(new Title(), true);
	}
}



void Result::ChangeSceneSpriteRender(ID3D11DeviceContext* dc)
{
	if (black_band_timer == 0.0f) return;

	// 黒帯
	constexpr float scale = 300.0f;

	black_band->Render(dc, 0, 0, 1920, scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
	black_band->Render(dc, 0, 1080, 1920, -scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
}
