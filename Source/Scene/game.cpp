

#include "game.h"
#include "result.h"
#include "sceneManager.h"
#include "common.h"
#include "Input/Input.h"
#include "Camera.h"
#include "CameraController.h"

// �ǉ�
#include "gameSystem.h"
#include "stageManager.h"
#include "easy_math.h"
#include "audioManager.h"

extern int scene_tag;
int attck_select_state = 0;


void Game::Update(float elapsedTime)
{

	// �V�[���ύX
	ChangeScene(elapsedTime);

	//
	scene_tag = Scene::SCENE_GAME;

	// �|�[�Y
	if (pause->Update(elapsedTime)) return;

	BGMStart();

	//	��	�@���͏����Ƃ����낢�돑���@	��	�@//


	// TODO: �Q�[������

	GameSystem::Instance().HitStopUpdate(elapsedTime);

	StageManager::Instance().Update(elapsedTime);

	EnemyManager::Instance().Update(elapsedTime, player->GetPosition());

	EnemyManager::Instance().SortLengthSq(player->GetPosition());

	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 12.0f;
	//target.z -= 10.0f;
	cameraController->SetTarget(target);
	cameraController->Update(elapsedTime);

	stress++;
	if (stress > 512)
		stress = 512;

	//�U���I��
	Mouse& mouse = Input::Instance().GetMouse();
	if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)
		attck_select_state++;
	if (attck_select_state > 2)
		attck_select_state = 0;
	//�U������
	switch (attck_select_state)
	{
	case 0:
		attacSelectPos.x = 20;
		frameScale1 = 1.2f;
		frameScale2 = 1.0f;
		frameScale3 = 1.0f;
		break;
	case 1:
		attacSelectPos.x = 20 + 160;
		frameScale1 = 1.0f;
		frameScale2 = 1.2f;
		frameScale3 = 1.0f;
		break;
	case 2:
		attacSelectPos.x = 20 + 160*2;
		frameScale1 = 1.0f;
		frameScale2 = 1.0f;
		frameScale3 = 1.2f;
		break;
	}

	player->Update(elapsedTime);


	//DirectX::XMFLOAT3 player_pos = player->GetPosition();
	//CameraController::Instance()->SetTarget(float3SUM({player_pos.x, player_pos.y + 3, player_pos.z}, float3Scaling(player->GetFront(), 4.5f)));
	//CameraController::Instance()->SetTarget({ player->GetPosition().x, player->GetPosition().y + 10, player->GetPosition().z - 3});
	//CameraController::Instance()->Update(elapsedTime);


	GameSystem::Instance().Update(elapsedTime);

}


void Game::ModelRender(ID3D11DeviceContext* dc, Shader* shader)
{
	/* 3D���f���̕`�� */
	StageManager::Instance().ModelRender(dc, shader);

	player->Render(dc, shader);

	// �G�l�~�[�`��
	EnemyManager::Instance().Render(dc, shader);
}


void Game::SpriteRender(ID3D11DeviceContext* dc)
{
	/* 2D�X�v���C�g�̕`�� */
	GameSystem::Instance().SpriteRender(dc);

	pause->SpriteRender(dc);

	//UI�����_�[
	{
		//HP�t���[��
		HpBarFrame->Render2(dc,
			20, 30,				//�\���ʒu
			1.0f, 1.0f,			//�X�P�[��
			0, 0,				// �摜�؂蔲���ʒu
			640, 64,			// �摜�؂蔲���T�C�Y
			0, 0,				//�摜��_
			0,					// �p�x
			1, 1, 1, 1);		// �F���(r,g,b,a)

		//HP
		HpBar->Render2(dc,
			20, 30,
			1.0f, 1.0f,
			0, 0,
			640, 64,
			0, 0,
			0,
			1, 1, 1, 1);
		//HP�A�C�R��
		HpIcon->Render2(dc,
			26, 38,
			1.0f, 1.0f,
			0, 0,
			83, 52,
			0, 0,
			0,
			1, 1, 1, 1);


		//�X�g���X�t���[��
		StressBarFrame->Render2(dc,
			20, 95,
			1.0f, 1.0f,
			0, 0,
			512, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//�X�g���X
		StressBar->Render2(dc,
			20, 95,
			1.0f, 1.0f,
			0, 0,
			stress, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//�X�g���X�A�C�R��
		StressIcon->Render2(dc,
			26, 103,
			1.0f, 1.0f,
			0, 0,
			48, 49,
			0, 0,
			0,
			1, 1, 1, 1);

		//�����̃t���[��
		StressBarFrame->Render(dc,
			20, 160, 384, 64,
			0, 0, 384, 64,
			0,
			1, 1, 1, 1);

		//����
		GoldBar->Render2(dc,
			20, 160,
			1.0f, 1.0f,
			0, 0,
			384, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//�����A�C�R��
		GoldIcon->Render2(dc,
			28, 168,
			1.0f, 1.0f,
			0, 0,
			59, 51,
			0, 0,
			0,
			1, 1, 1, 1);

		//�~�j�}�b�v
		Minimap->Render2(dc,
			1600, 10,
			1.0f, 1.0f,
			0, 0,
			248, 248,
			0, 0,
			0,
			1, 1, 1, 1);

		//��t���[��
		CastlebarFrame->Render2(dc,
			1600, 276,
			1.0f, 1.0f,
			0, 0,
			256, 64,
			0, 0,
			0,
			1, 1, 1, 1);
		//��
		Castlebar->Render2(dc,
			1600, 276,
			1.0f, 1.0f,
			0, 0,
			256, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//�U���X���b�g
		AttackSlot->Render2(dc,
			20, 850,
			1.0f, 1.0f,
			0, 0,
			480, 160,
			0, 0,
			0,
			1, 1, 1, 1);

		//�U���I��
		AttackSelect->Render2(dc,
			attacSelectPos.x, 850,
			1.0f, 1.0f,
			0, 0,
			160, 160,
			0, 0,
			0,
			1, 1, 1, 1);
	}

	ClearedSpriteRender(dc);
}


void Game::DeInit()
{

	// �v���C���[�I����
	safe_delete(player);

	// �G�̔z�u�I����
	safe_delete(enemy_Arrangement);

	// �G�l�~�[�I����
	EnemyManager::Instance().Clear();

	StageManager::Instance().AllClear();

	//�J�����R���g���[���[�I����
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
}


void Game::Load()
{
	pause		= std::make_unique<Pause>(this);

	// �v���C���[������
	player = new Player();
	player->SetPosition(DirectX::XMFLOAT3(0, 0, 15));

	enemy_Arrangement = new Enemy_Arrangement();
	enemy_Arrangement->enemy_produce();

	black_band = std::make_unique<Sprite>();

	StageManager::Instance().AddStage(new StageRoom());

	//UI������
	HpBar					= std::make_unique<Sprite>("Data/Sprite/G_HP.png");
	HpBarFrame				= std::make_unique<Sprite>("Data/Sprite/HP����.png");
	HpIcon					= std::make_unique<Sprite>("Data/Sprite/I_HP.png");
	StressBar				= std::make_unique<Sprite>("Data/Sprite/�X�g���X.png");
	StressBarFrame			= std::make_unique<Sprite>("Data/Sprite/�X�g���X����.png");
	StressIcon				= std::make_unique<Sprite>("Data/Sprite/I_box.png");
	GoldBar					= std::make_unique<Sprite>("Data/Sprite/G_gold.png");
	GoldIcon				= std::make_unique<Sprite>("Data/Sprite/I_gold.png");
	Minimap					= std::make_unique<Sprite>("Data/Sprite/map.png");
	Castlebar				= std::make_unique<Sprite>("Data/Sprite/��.png");
	CastlebarFrame			= std::make_unique<Sprite>("Data/Sprite/�鍕��.png");
	AttackSlot				= std::make_unique<Sprite>("Data/Sprite/G_attack.png");
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


void Game::CameraSet()
{
#if false
	//�J���������ݒ�
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

	//�J���������ݒ�
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();	//&���Y��Ȃ�!!
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

	//�J�����R���g���[���[������
	cameraController = new CameraController();
}


void Game::ChangeScene(float elapsedTime)
{
#if _DEBUG
	ChangeNextScene(new Result(), GamePad::BTN_A, false);
#endif

	if (GameSystem::Instance().NowTime() > 0.0f && smallest == false) return;


	// ���т̍X�V
	black_band_timer += 1.0f * elapsedTime;


	// ���т��~�肫������
	if (black_band_timer >= 1.4f)
	{
		// �c�莞�Ԃ��[���ɂȂ����ۃV�[���J�ڂ�����
		ChangeNextScene(new Result(), false);

		if(bgm_caution == false)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
		if (bgm_caution == true)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_SPEED)->Stop();
	}
}


void Game::ClearedSpriteRender(ID3D11DeviceContext* dc)
{
	if (black_band_timer == 0.0f) return;

	// ����
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
