

#include "tutorial.h"
#include "game.h"
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



void Tutorial::Update(float elapsedTime)
{

	//	↓	　入力処理とかいろいろ書く　	↓	　//

	// TODO: チュートリアル処理

	GamePad& gamePad = Input::Instance().GetGamePad();

	if (explaining && explanation < 18 && gamePad.GetButtonDown() & GamePad::BTN_SPACE)
		explanation++;

	StageManager::Instance().Update(elapsedTime);

	// エネミー更新処理
	EnemyManager::Instance().Update(elapsedTime, player->GetPosition());

	player->Update(elapsedTime, explaining);

	//CameraController::Instance()->SetTarget(float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), 5.0f)));
	//CameraController::Instance()->Update(elapsedTime, explaining);

	// 行動制限
	if (player->GetPosition().x > 15.0f) player->SetPosition({ 15.0f, player->GetPosition().y, player->GetPosition().z });
	if (player->GetPosition().z > 30.0f) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, 30.0f });
	if (player->GetPosition().x < -15.0f) player->SetPosition({ -15.0f, player->GetPosition().y, player->GetPosition().z });
	if (player->GetPosition().z < -15.0f) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, -15.0f });

	switch (tutorial_state)
	{
	case PLAYER_MOVE:
		if (explanation == 4)
		{
			explaining = false;
			if (gamePad.GetButtonDown() & GamePad::BTN_UP)		ws[0] = true;
			if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)	ws[1] = true;
			if (ws[0] && ws[1]) End_of_explanation(elapsedTime); // クリア条件を満たしたら次のstateへ
		}
		break;
	case CAMERA_MOVE:
		if (explanation == 7)
		{
			explaining = false;
			if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)	adik[0] = true;
			if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)	adik[1] = true;
			if (gamePad.GetButtonDown() & GamePad::BTN_I)		adik[2] = true;
			if (gamePad.GetButtonDown() & GamePad::BTN_K)		adik[3] = true;
			
			if (adik[0] && adik[1] && adik[2] && adik[3])
			{
				End_of_explanation(elapsedTime);
				enemy_Arrangement->enemy_produce(Enemy_Arrangement::csv_file_num::TUTORIAL_NORMAL);
			}
		}
		break;
	case MERIT:
		if (explanation == 11)
		{
			explaining = false;
			if (EnemyManager::Instance().GetEnemyCount() <= 0)
			{
				End_of_explanation(elapsedTime);
				enemy_Arrangement->enemy_produce(Enemy_Arrangement::csv_file_num::TUTORIAL_BOMB);
			}
		}
		break;
	case DEMERIT:
		if (explanation == 15)
		{
			explaining = false;
			if (EnemyManager::Instance().GetEnemyCount() <= 0) End_of_explanation(elapsedTime);
		}
		break;
	case SHELL_SIZE:
		if (explanation == 18)
		{
			tutorial_state++;
			//End_of_explanation(elapsedTime);
		}
		break;
	case END:
		// シーン変更
		if(gamePad.GetButtonDown() & GamePad::BTN_SPACE)
		{ 
			ChangeNextScene(new Game()); // 急にシーンが変わると不自然なので任意のタイミングで変える
			AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
			AudioManager::Instance().GetAudio(Audio_INDEX::SE_SUCCESS)->Stop();
		}
		
		break;
	default:
		break;
	}
}


void Tutorial::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3Dモデルの描画 */
	StageManager::Instance().ModelRender(dc, shader);

	player->Render(dc, shader);

	// エネミー描画
	EnemyManager::Instance().Render(dc, shader);
}


void Tutorial::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2Dスプライトの描画 */

	float spr_explanationWidth = CAST_F(spr_explanation->GetTextureWidth());
	float spr_explanationHeight = CAST_F(spr_explanation->GetTextureHeight());

	if (explaining)
	{
		if (explanation == 18)
		{
			spr_start->Render2(dc,
				0, 0,						// 表示位置
				1.0f, 1.0f,									// スケール
				0, 0,										// 画像切り抜き位置
				1920, 1080,				// 画像切り抜きサイズ
				0, 0,	// 画像基準点
				angle,										// 角度
				1, 1, 1, 1);								// 色情報(r,g,b,a)
		}
		else
		{
			spr_space->Render2(dc,
				0, 0,						// 表示位置
				1.0f, 1.0f,									// スケール
				0, 0,										// 画像切り抜き位置
				1920, 1080,				// 画像切り抜きサイズ
				0, 0,	// 画像基準点
				angle,										// 角度
				1, 1, 1, 1);								// 色情報(r,g,b,a)
		}
	}

	spr_explanation->Render2(dc,
		320, 100,						// 表示位置
		1.0f, 1.0f,									// スケール
		0, 240 * explanation,										// 画像切り抜き位置
		spr_explanationWidth, 240,				// 画像切り抜きサイズ
		0, 0,	// 画像基準点
		angle,										// 角度
		1, 1, 1, 1);								// 色情報(r,g,b,a)
}


void Tutorial::DeInit()
{
	// プレイヤー終了化
	safe_delete(player);

	// 敵の配置終了化
	safe_delete(enemy_Arrangement);

	// エネミー終了化
	EnemyManager::Instance().Clear();

	StageManager::Instance().AllClear();

	// 説明画像の初期化
	explanation = 0;

	for (int i = 0; i < 2; i++)
	{
		ws[i] = false;
	}
	
	for (int i = 0; i < 4; i++)
	{
		adik[i] = false;
	}
}


void Tutorial::Set()
{
	back_color = { 1,0,0,1 };

	CameraSet();

	AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Play(true);
}


void Tutorial::Load()
{
	spr_explanation = std::make_unique<Sprite>("Data/Sprite/チュートリアル説明文2.png");
	spr_space = std::make_unique<Sprite>("Data/Sprite/space（チュートリアル）.png");
	spr_start = std::make_unique<Sprite>("Data/Sprite/スタート（チュートリアル）.png");

	// プレイヤー初期化
	player = new Player();
	player->ResetTransform();

	enemy_Arrangement = new Enemy_Arrangement();

	StageManager::Instance().AddStage(new StageRoom());
}


void Tutorial::ImGui()
{
	ImGui::Text("scene : Tutorial");

	//ImGui::SliderFloat("camera range", &CameraController::Instance()->GerRange(), 1, 1000);

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());
	ImGui::Text("tutorial_state : %d", tutorial_state);
}


void Tutorial::DebugRender()
{
	player->DrawDebugPrimitive();
	EnemyManager::Instance().DrawDebugPrimitive();
}


void Tutorial::CameraSet()
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

	//CameraController::Instance()->init();
	//CameraController::Instance()->SetCameraBehavior(CAMERA::PADCONTROL);
	//CameraController::Instance()->SetRange(15.0f);
}

void Tutorial::End_of_explanation(float elapsedTime)
{
	AudioManager::Instance().GetAudio(Audio_INDEX::SE_SUCCESS)->Stop();
	AudioManager::Instance().GetAudio(Audio_INDEX::SE_SUCCESS)->Play(false);

	tutorial_state++;

	player->ResetTransform();

	CameraSet();
	
	
	if(explanation < 18) explanation++;
	explaining = true;
}