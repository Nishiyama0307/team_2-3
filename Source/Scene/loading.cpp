

#include <thread>
#include "loading.h"
#include "common.h"
#include "Input/Input.h"



void Loading::Update(float elapsedTime)
{

	// シーン変更
	{

	}


	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: 読み込み処理

	angle += elapsedTime * 200.0f;/* TEST: 後で消す */

	LoadTimer +=  10.0f;
	if (LoadTimer > 150.0f)
	{
		//texPos.x = 464*1;
		texPos.x = 1920*1;
	}
	if (LoadTimer > 300.0f)
	{
		//texPos.x = 464*2;
		texPos.x = 1920 * 2;
	}
	if (LoadTimer > 450.0f)
	{
		texPos.x = 0;
		LoadTimer = 0;
	}

	if (scene->IsSceneLoaded())
	{
		ChangeNextScene(scene, false);
		scene = nullptr;
	}
}


void Loading::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{

}


void Loading::SpriteRender(ID3D11DeviceContext* dc)
{
	Graphics& graphics = Graphics::Instance();

	float screenWidth = CAST_F(graphics.GetScreenWidth());
	float screenHeight = CAST_F(graphics.GetScreenHeight());
	float textureWidth = CAST_F(spr_icon->GetTextureWidth());
	float textureHeight = CAST_F(spr_icon->GetTextureHeight());
	float positionX = screenWidth - textureWidth;
	float positionY = screenHeight - textureHeight;

	//spr_back->Render2(dc,
	//	0, 0,						// 表示位置
	//	1.0f, 1.0f,									// スケール
	//	0, 0,										// 画像切り抜き位置
	//	1920, 1080,				// 画像切り抜きサイズ
	//	0, 0,	// 画像基準点
	//	0,										// 角度
	//	1, 1, 1, 1);								// 色情報(r,g,b,a)
	//
	//spr_icon->Render2(dc,
	//	positionX, positionY,						// 表示位置
	//	1.0f, 1.0f,									// スケール
	//	0, 0,										// 画像切り抜き位置
	//	textureWidth, textureHeight,				// 画像切り抜きサイズ
	//	textureWidth * 0.5f, textureHeight * 0.5f,	// 画像基準点
	//	angle,										// 角度
	//	1, 1, 1, 1);								// 色情報(r,g,b,a)

	//ロード
	spr_load->Render2(dc,
		0, 0,
		1.0f, 1.0f,
		0, 0,
		1920, 1080,
		0, 0,
		(0),
		1, 1, 1, 1);

	//spr_load_word->Render2(dc,
	//	1350, 900,
	//	/*1000, 500,*/
	//	1.0f, 1.0f,
	//	texPos.x, texPos.y,
	//	464, 140,
	//	0, 0,
	//	0,
	//	1, 1, 1, 1);

	spr_load_word->Render2(dc,
		/*1350, 900,*/
		0, -30,
		1.0f, 1.0f,
		texPos.x, texPos.y,
		1920, 1080,
		0, 0,
		0,
		1, 1, 1, 1);
}


void Loading::DeInit()
{
	safe_delete(spr_icon);

	if (scene == nullptr) return;
	scene->DeInit();
	safe_delete(scene);
}


void Loading::Set()
{
	back_color = { 1,1,1,1 };

	std::thread thread(LoadingThread, this, GetSceneManager());

	thread.detach();

	texPos.x = 0;
	texPos.y = 0;
	LoadTimer = 0;
}


void Loading::Load()
{
	spr_back = std::make_unique<Sprite>("Data/Sprite/タイトルロゴなし背景画像（リザルト）.png");
	spr_icon = new Sprite("Data/Sprite/ローディング.png");

	spr_load		= std::make_unique<Sprite>("Data/Sprite/scene/road.png");
	spr_load_word	= std::make_unique<Sprite>("Data/Sprite/scene/road_moji3.png");
}


void Loading::ImGui()
{
	ImGui::Text("scene : Loading");
}

void Loading::LoadingThread(Loading* load, SceneManager* scene_manager_)
{
	CoInitialize(nullptr);

	load->scene->Init(scene_manager_);

	CoUninitialize();

	load->scene->SetSceneLoaded();
}