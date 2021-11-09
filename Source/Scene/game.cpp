

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

extern int scene_tag;
int attck_select_state = 0;
extern bool f1;

void Game::Update(float elapsedTime)
{
	// シーン変更
	ChangeScene(elapsedTime);

	//
	scene_tag = Scene::SCENE_GAME;

	// ポーズ
	if (pause->Update(elapsedTime)) return;
	

	BGMStart();

	//	↓	　入力処理とかいろいろ書く　	↓	　//


	// TODO: ゲーム処理

	GameSystem::Instance().HitStopUpdate(elapsedTime);

	StageNumUpdate();

	StageManager::Instance().Update(elapsedTime, stage_num);

	EnemyManager::Instance().Update(elapsedTime, player->GetPosition(), stage_num);

	EnemyManager::Instance().SortLengthSq(player->GetPosition());

	//カメラコントローラー更新処理
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 12.0f;
	//target.z -= 10.0f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	stress+= 0.1f;
	if (stress > 512)
		stress = 512;

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
		attackPosx1 = 150;
		attackPosx2 = 350;
		attackPosx3 = 20;

		attackPosY1 = 850;
		attackPosY2 = 900;
		attackPosY3 = 900;

		attacSelectPos.x = 20;
		AttackSlotScale1 = 1.2f;
		AttackSlotScale2 = 0.7f;
		AttackSlotScale3 = 0.7f;
		break;
	case 1:
		attackPosx1 = 20;
		attackPosx2 = 150;
		attackPosx3 = 350;

		attackPosY1 = 900;
		attackPosY2 = 850;
		attackPosY3 = 900;

		attacSelectPos.x = 20 + 160;
		AttackSlotScale1 = 0.7f;
		AttackSlotScale2 = 1.2f;
		AttackSlotScale3 = 0.7f;
		break;
	case 2:
		attackPosx1 = 350;
		attackPosx2 = 20;
		attackPosx3 = 150;

		attackPosY1 = 900;
		attackPosY2 = 900;
		attackPosY3 = 850;

		attacSelectPos.x = 20 + 160 * 2;
		AttackSlotScale1 = 0.7f;
		AttackSlotScale2 = 0.7f;
		AttackSlotScale3 = 1.2f;
		break;
	}

	player->Update(elapsedTime);

#ifdef _DEBUG
	// デバッグのみのワープキー
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_B) // xキー
		//player->SetPosition(DirectX::XMFLOAT3(0, 0, -170)); // 本番
		player->SetPosition(DirectX::XMFLOAT3(0, 0, -100)); // test

	if (gamePad.GetButtonDown() & GamePad::BTN_X) // cキー
		//player->SetPosition(DirectX::XMFLOAT3(0, 0, 2600)); // 本番
		player->SetPosition(DirectX::XMFLOAT3(0, 0, 2575)); // test

#endif

	//DirectX::XMFLOAT3 player_pos = player->GetPosition();
	//CameraController::Instance()->SetTarget(float3SUM({player_pos.x, player_pos.y + 3, player_pos.z}, float3Scaling(player->GetFront(), 4.5f)));
	//CameraController::Instance()->SetTarget({ player->GetPosition().x, player->GetPosition().y + 10, player->GetPosition().z - 3});
	//CameraController::Instance()->Update(elapsedTime);


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
		//HPアイコン
		HpIcon->Render2(dc,
			26, 38,
			1.0f, 1.0f,
			0, 0,
			83, 52,
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
			stress, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//ストレスアイコン
		StressIcon->Render2(dc,
			26, 103,
			1.0f, 1.0f,
			0, 0,
			48, 49,
			0, 0,
			0,
			1, 1, 1, 1);

		//お金のフレーム
		StressBarFrame->Render(dc,
			20, 160, 384, 64,
			0, 0, 384, 64,
			0,
			1, 1, 1, 1);

		//お金
		GoldBar->Render2(dc,
			20, 160,
			1.0f, 1.0f,
			0, 0,
			384, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//お金アイコン
		GoldIcon->Render2(dc,
			28, 168,
			1.0f, 1.0f,
			0, 0,
			59, 51,
			0, 0,
			0,
			1, 1, 1, 1);

		//ミニマップ
		Minimap->Render2(dc,
			1600, 10,
			1.0f, 1.0f,
			0, 0,
			248, 248,
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

		//攻撃スロット1
		AttackSlot1->Render2(dc,
			attackPosx1, attackPosY1,
			1.0f * AttackSlotScale1, 1.0f * AttackSlotScale1,
			0, 0,
			153, 148,
			0, 0,
			0,
			1, 1, 1, 1);

		//攻撃スロット2
		AttackSlot2->Render2(dc,
			attackPosx2, attackPosY2,
			1.0f * AttackSlotScale2, 1.0f * AttackSlotScale2,
			0, 0,
			153, 148,
			0, 0,
			0,
			1, 1, 1, 1);

		//攻撃スロット3
		AttackSlot3->Render2(dc,
			attackPosx3, attackPosY3,
			1.0f * AttackSlotScale3, 1.0f * AttackSlotScale3,
			0, 0,
			153, 148,
			0, 0,
			0,
			1, 1, 1, 1);

		//攻撃選択
		//AttackSelect->Render2(dc,
		//	150, 850,
		//	1.2f, 1.2f,
		//	0, 0,
		//	160, 160,
		//	0, 0,
		//	0,
		//	1, 1, 1, 1);
	}

	pause->SpriteRender(dc);
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

	//カメラコントローラー終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}
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
	stress = 0;

	stage_num = STAGENUM::N_STAGE1_VOLCANO;

	attackPosY1 = 850;
	attackPosY2 = 850;
	attackPosY3 = 850;
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

	StageManager::Instance().AddStage(new Stage1_Volcano());
	StageManager::Instance().AddStage(new Stage2_Desert());
	StageManager::Instance().AddStage(new Stage3_Volcano());
	StageManager::Instance().AddStage(new Stage4_Volcano());
	StageManager::Instance().AddStage(new castle());

	//UI初期化
	HpBar					= std::make_unique<Sprite>("Data/Sprite/G_HP.png");
	HpBarFrame				= std::make_unique<Sprite>("Data/Sprite/HP黒帯.png");
	HpIcon					= std::make_unique<Sprite>("Data/Sprite/I_HP.png");
	StressBar				= std::make_unique<Sprite>("Data/Sprite/ストレス.png");
	StressBarFrame			= std::make_unique<Sprite>("Data/Sprite/ストレス黒帯.png");
	StressIcon				= std::make_unique<Sprite>("Data/Sprite/I_box.png");
	GoldBar					= std::make_unique<Sprite>("Data/Sprite/G_gold.png");
	GoldIcon				= std::make_unique<Sprite>("Data/Sprite/I_gold.png");
	Minimap					= std::make_unique<Sprite>("Data/Sprite/map.png");
	Castlebar				= std::make_unique<Sprite>("Data/Sprite/城.png");
	CastlebarFrame			= std::make_unique<Sprite>("Data/Sprite/城黒帯.png");
	AttackSlot1				= std::make_unique<Sprite>("Data/Sprite/AT1.png");
	AttackSlot2				= std::make_unique<Sprite>("Data/Sprite/AT2.png");
	AttackSlot3				= std::make_unique<Sprite>("Data/Sprite/AT3.png");
	AttackSelect			= std::make_unique<Sprite>("Data/Sprite/attack_waku_R.png");
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");

	//ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());

	DirectX::XMFLOAT3 pos = player->GetPosition();
	ImGui::Text("player pos %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);

	float length;
	DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&player->GetVelocity())));
	ImGui::Text("velo length : %.1f", length);

	DirectX::XMFLOAT3 velo = player->GetVelocity();
	ImGui::Text("player velo %.1f, %.1f, %.1f", velo.x, velo.y, velo.z);

	if (ImGui::CollapsingHeader("enemy", ImGuiTreeNodeFlags_DefaultOpen))
	{
		EnemyManager::Instance().DrawDebugGUI();
	}
}


void Game::DebugRender()
{
	player->DrawDebugPrimitive();
	EnemyManager::Instance().DrawDebugPrimitive();
}

void Game::StageNumUpdate()
{
	//constexpr int STAGE2_start_position{ 565 };
	//constexpr int STAGE3_start_position{ 1240 };
	//constexpr int STAGE4_start_position{ 1920 };

	if (kStage3_Start_Position > player->GetPosition().z && player->GetPosition().z >= kStage2_Start_Position) stage_num = N_STAGE2_DESERT; // 1240 > p >= 565
	if (player->GetPosition().z < kStage2_Start_Position) stage_num = N_STAGE1_VOLCANO;						  // p < 565

	if (kStage4_Start_Position > player->GetPosition().z && player->GetPosition().z >= kStage3_Start_Position) stage_num = N_STAGE3_;	  // 1910 > p >= 1240
	if (kStage2_Start_Position < player->GetPosition().z && player->GetPosition().z < kStage3_Start_Position) stage_num = N_STAGE2_DESERT; // 565 < p < 1240

	if (player->GetPosition().z >= kStage4_Start_Position) stage_num = N_STAGE4_;							  // p >= 1920
	if (kStage3_Start_Position < player->GetPosition().z && player->GetPosition().z < kStage4_Start_Position) stage_num = N_STAGE3_;		  // 1240 < p < 1920
}

void Game::CameraSet()
{
#if false
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
#endif

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();	//&つけ忘れない!!
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

	//カメラコントローラー初期化
	cameraController = new CameraController();
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
