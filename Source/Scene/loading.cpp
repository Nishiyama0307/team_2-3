

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

	spr_back->Render2(dc,
		0, 0,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		1920, 1080,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		0,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)

	spr_icon->Render2(dc,
		positionX, positionY,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 0,										// 画像切り抜き位置
		textureWidth, textureHeight,				// 画像切り抜きサイズ
		textureWidth * 0.5f, textureHeight * 0.5f,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)
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
}


void Loading::Load()
{
	spr_back = std::make_unique<Sprite>("Data/Sprite/タイトルロゴなし背景画像（リザルト）.png");
	spr_icon = new Sprite("Data/Sprite/ローディング.png");
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