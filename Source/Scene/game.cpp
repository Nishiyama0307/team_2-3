

#include "game.h"
#include "result.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"

// 追加
#include "gameSystem.h"
#include "stageManager.h"
#include "easy_math.h"
#include "audioManager.h"



void Game::Update(float elapsedTime)
{

	// シーン変更
	ChangeScene(elapsedTime);

	// ポーズ
	if (pause->Update(elapsedTime)) return;

	BGMStart();

	//	↓	　入力処理とかいろいろ書く　	↓	　//


	// TODO: ゲーム処理

	GameSystem::Instance().HitStopUpdate(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime);

	EnemyManager::Instance().SortLengthSq(player->GetPosition());


	//攻撃選択
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
		attck_select_state++;
	if (attck_select_state > 2)
		attck_select_state = 0;
	//攻撃分岐
	switch (attck_select_state)
	{
	case 0:
		attacSelectPos.x = 38;
		break;
	case 1:
		attacSelectPos.x = 198;
		break;

	case 2:
		attacSelectPos.x = 358;
		break;
	}

	player->Update(elapsedTime);


	//DirectX::XMFLOAT3 player_pos = player->GetPosition();
	//CameraController::Instance()->SetTarget(float3SUM({player_pos.x, player_pos.y + 3, player_pos.z}, float3Scaling(player->GetFront(), 4.5f)));
	CameraController::Instance()->SetTarget({ player->GetPosition().x, player->GetPosition().y + 10, player->GetPosition().z - 3});
	CameraController::Instance()->Update(elapsedTime);


	GameSystem::Instance().Update(elapsedTime);

}


void Game::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	StageManager::Instance().ModelRender(dc, shader);

	player->Render(dc, shader);

	// エネミー描画
	EnemyManager::Instance().Render(dc, shader);
}


void Game::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */
	GameSystem::Instance().SpriteRender(dc);

	pause->SpriteRender(dc);

	//UIレンダー
	{
		//HPフレーム
		HpBarFrame->Render2(dc,
			20, 30,				//表示位置
			1.0f, 1.0f,			//スケール
			0, 0,				// 画像切り抜き位置
			640, 64,			// 画像切り抜きサイズ
			0, 0,				//画像基準点
			0,					// 角度
			1, 1, 1, 1);		// 色情報(r,g,b,a)

		//HP
		HpBar->Render2(dc,
			20, 30,
			1.0f, 1.0f,
			0, 0,
			640, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//ストレスフレーム
		StressBarFrame->Render2(dc,
			20, 95,
			1.0f, 1.0f,
			0, 0,
			512, 64,
			0, 0,
			0,
			1, 1, 1, 1);
		//ストレス
		StressBar->Render2(dc,
			20, 95,
			1.0f, 1.0f,
			0, 0,
			512, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//お金
		GoldBar->Render2(dc,
			20, 159,
			1.0f, 1.0f,
			0, 0,
			384, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//ミニマップ
		Minimap->Render2(dc,
			1600, 10,
			1.0f, 1.0f,
			0, 0,
			256, 256,
			0, 0,
			0,
			1, 1, 1, 1);

		//城フレーム
		CastlebarFrame->Render2(dc,
			1600, 276,
			1.0f, 1.0f,
			0, 0,
			256, 64,
			0, 0,
			0,
			1, 1, 1, 1);
		//城
		Castlebar->Render2(dc,
			1600, 276,
			1.0f, 1.0f,
			0, 0,
			256, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//攻撃スロット
		AttackSlot->Render2(dc,
			20, 850,
			1.0f, 1.0f,
			0, 0,
			480, 160,
			0, 0,
			0,
			1, 1, 1, 1);

		//攻撃選択
		AttackSelect->Render2(dc,
			attacSelectPos.x, attacSelectPos.y,
			1.0f, 1.0f,
			0, 0,
			128, 128,
			0, 0,
			0,
			1, 1, 1, 1);
	}

	ClearedSpriteRender(dc);
}


void Game::DeInit()
{

	// プレイヤー終了化
	safe_delete(player);

	// 敵の配置終了化
	safe_delete(enemy_Arrangement);

	// エネミー終了化
	EnemyManager::Instance().Clear();

	StageManager::Instance().AllClear();
}
 

void Game::Set()
{
	back_color = { 0,0,1,1 };

	CameraSet();

	GameSystem::Instance().GameStart();

	black_band_timer = 0.0f;

	smallest = false;

	attacSelectPos.x = 38;
	attacSelectPos.y = 865;

	attck_select_state = 0;
}


void Game::Load()
{
	pause		= std::make_unique<Pause>(this);

	// プレイヤー初期化
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(0, 0, 15));

	enemy_Arrangement = new Enemy_Arrangement();
	enemy_Arrangement->enemy_produce();

	black_band = std::make_unique<Sprite>();

	StageManager::Instance().AddStage(new StageRoom());

	//UI初期化
	HpBar					= std::make_unique<Sprite>("Data/Sprite/HP.png");
	HpBarFrame				= std::make_unique<Sprite>("Data/Sprite/HP黒帯.png");
	StressBar				= std::make_unique<Sprite>("Data/Sprite/ストレス.png");
	StressBarFrame			= std::make_unique<Sprite>("Data/Sprite/ストレス黒帯.png");
	GoldBar					= std::make_unique<Sprite>("Data/Sprite/ゴールド.png");
	Minimap					= std::make_unique<Sprite>("Data/Sprite/ミニマップ.png");
	Castlebar				= std::make_unique<Sprite>("Data/Sprite/城.png");
	CastlebarFrame			= std::make_unique<Sprite>("Data/Sprite/城黒帯.png");
	AttackSlot				= std::make_unique<Sprite>("Data/Sprite/攻撃スロット.png");
	AttackSelect			= std::make_unique<Sprite>("Data/Sprite/丸.png");
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");

	ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());

	DirectX::XMFLOAT3 pos = player->GetPosition();
	ImGui::Text("player pos %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);

	float length;
	DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&player->GetVelocity())));
	ImGui::Text("velo length : %.1f", length);

	DirectX::XMFLOAT3 velo = player->GetVelocity();
	ImGui::Text("player velo %.1f, %.1f, %.1f", velo.x, velo.y, velo.z);
}


void Game::DebugRender()
{
	player->DrawDebugPrimitive();
	EnemyManager::Instance().DrawDebugPrimitive();
}


void Game::CameraSet()
{
	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 10, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	CameraController::Instance()->init();
	CameraController::Instance()->SetCameraBehavior(CAMERA::PADCONTROL);
	CameraController::Instance()->SetRange(15.0f);
}


void Game::ChangeScene(float elapsedTime)
{
#if _DEBUG
	ChangeNextScene(new Result(), GamePad::BTN_A, false);
#endif

	if (GameSystem::Instance().NowTime() > 0.0f && smallest == false) return;


	// 黒帯の更新
	black_band_timer += 1.0f * elapsedTime;


	// 黒帯が降りきったら
	if (black_band_timer >= 1.4f)
	{
		// 残り時間がゼロになった際シーン遷移をする
		ChangeNextScene(new Result(), false);

		if(bgm_caution == false)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
		if (bgm_caution == true)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_SPEED)->Stop();
	}
}


void Game::ClearedSpriteRender(ID3D11DeviceContext* dc)
{
	if (black_band_timer == 0.0f) return;

	// 黒帯
	constexpr float scale = 300.0f;

	black_band->Render(dc, 0, 0, 1920, scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
	black_band->Render(dc, 0, 1080, 1920, -scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 1, 1, 1, 1);
}


void Game::BGMStart()
{
	if (bgm_normal == false && bgm_caution == false)
	{
		bgm_normal = true;
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Play(true);
	}

	if (bgm_normal == true && bgm_caution == false && GameSystem::Instance().NowTime() <= 20.0f)
	{
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
		bgm_caution = true;
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_SPEED)->Play(true);
	}
}
