

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

#include "title.h"

extern int scene_tag;
int attck_select_state = 0;
extern bool f1;
extern int result;

bool is_do_tutorial = { false }; // �`���[�g���A�����s�����ǂ���

void Game::Update(float elapsedTime)
{
	// �V�[���ύX
	ChangeScene(elapsedTime);

	//
	scene_tag = Scene::SCENE_GAME;

	// �|�[�Y
	if (pause->Update(elapsedTime))
	{
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK1)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK2)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK3)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DAMAGE)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DIE)->Stop();
		return;
	}

	if (is_do_tutorial == false) countdown->Update(elapsedTime);

	BGMStart();

	//	��	�@���͏����Ƃ����낢�돑���@	��	�@//

	// TODO: �Q�[������

	//�Q�[���N���A
	if (player->GetPosition().z > 2545 && player->GetPosition().x < 25.0f && player->GetPosition().x > -25.0f)
	{
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK1)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK2)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK3)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DAMAGE)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DIE)->Stop();

		is_fadeSprite = true;
		fade_timer++;
		if (fade_timer > 120)
		{
			result = Game_clear;
			ChangeNextScene(new Result);
		}
	}
	if (countdown->NowCountDown() == false && brave_timer_ < brave_timelimit_) brave_timer_++;

	if (countdown->NowCountDown() == false && brave_timer_ >= brave_timelimit_ - 1) CastleHP -= 6.4 * elapsedTime;
	//CastleHP -= 100;
	if (CastleHP < 0) {
		CastleHP = 0;
	}

	//�v���C���[�����񂾂�Q�[���I�[�o�[��(��)
	if (player->animdeth) {
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK1)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK2)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK3)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DAMAGE)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DIE)->Stop();

		is_fadeSprite = true;
		fade_timer++;
		if (fade_timer > 120)
		{
			if (player->magumaDeath == true) {
				result = Game_over3;
				ChangeNextScene(new Result);
			}
			else
			{
				result = Game_over1;
				ChangeNextScene(new Result);
			}
		}
	}
	//��̗̑͂������Ȃ�����Q�[���I�[�o�[��(��)
	//if (CastleHP == 0) {
	//	AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK1)->Stop();
	//	AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK2)->Stop();
	//	AudioManager::Instance().GetAudio(Audio_INDEX::SE_PLAYER_ATTACK3)->Stop();
	//  AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DAMAGE)->Stop();
    //  AudioManager::Instance().GetAudio(Audio_INDEX::SE_ENEMY_DIE)->Stop();

	//	is_fadeSprite = true;
	//	fade_timer++;
	//	if (fade_timer > 120)
	//	{
	//		result = Game_over2;
	//		ChangeNextScene(new Result);
	//	}
	//}

	GameSystem::Instance().HitStopUpdate(elapsedTime);

	StageNumUpdate();

	StageManager::Instance().Update(elapsedTime, stage_num);

	if (countdown->NowCountDown() == false) EnemyManager::Instance().Update(elapsedTime, player->GetPosition(), stage_num);

	EnemyManager::Instance().SortLengthSq(player->GetPosition());

	//�J�����R���g���[���[�X�V����
	DirectX::XMFLOAT3 target = player->GetPosition();
	target.y += 12.0f;
#if 0
	cameraController->SetTarget(target);
#else
	cameraController->SetTarget(float3SUM({ target.x, target.y, target.z }, player->GetFront()));
#endif
	cameraController->Update(elapsedTime, explaining);

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

	if ((countdown->NowCountDown() == false && black_band_timer == 0.0f) || is_do_tutorial) player->Update(elapsedTime, stage_num, explaining);


	



#ifdef _DEBUG
	// �f�o�b�O�݂̂̃��[�v�L�[
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_B) // x�L�[
		//player->SetPosition(DirectX::XMFLOAT3(0, 0, -170)); // �{��
		player->SetPosition(DirectX::XMFLOAT3(0, 0, -100)); // test

	if (gamePad.GetButtonDown() & GamePad::BTN_X) // c�L�[
		//player->SetPosition(DirectX::XMFLOAT3(0, 0, 2600)); // �{��
		player->SetPosition(DirectX::XMFLOAT3(0, 0, 2575)); // test

#endif

	//DirectX::XMFLOAT3 player_pos = player->GetPosition();
	//CameraController::Instance()->SetTarget(float3SUM({player_pos.x, player_pos.y + 3, player_pos.z}, float3Scaling(player->GetFront(), 4.5f)));
	//CameraController::Instance()->SetTarget({ player->GetPosition().x, player->GetPosition().y + 10, player->GetPosition().z - 3});
	//CameraController::Instance()->Update(elapsedTime);

	// TODO: �`���[�g���A������
	if (is_do_tutorial)
	{
		if (explaining && explanation < 7 && mouse.GetButtonDown() & Mouse::BTN_LEFT)
		{
			explanation++;
			if(explanation != 8)
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_CLICK)->Play(false);
		}

		// �s������
		if (player->GetPosition().x > 25.0f) player->SetPosition({ 25.0f, player->GetPosition().y, player->GetPosition().z });
		if (player->GetPosition().x < -25.0f) player->SetPosition({ -25.0f, player->GetPosition().y, player->GetPosition().z });
		if (player->GetPosition().z > -120.0f) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, -120.0f });
		if (player->GetPosition().z < -155.0f) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, -155.0f });

		//�}�E�X����(�`���[�g���A��)
		//�}�E�X�{�b�N�X
		mousePos.x = mouse.GetPositionX() - 16;
		mousePos.y = mouse.GetPositionY() - 26;
		MouseBox.left = mousePos.x;
		MouseBox.top = mousePos.y;
		C_OffsetBox(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right, 32, 32);
		//���g���C����ׂ̃{�b�N�X
		RetryeBox.left = kakuninPos.x;
		RetryeBox.top = kakuninPos.y;
		C_OffsetBox(RetryeBox.top, RetryeBox.left, RetryeBox.bottom, RetryeBox.right, 100 * 2.2f, 50* 1.2f);
		//�I���{�b�N�X
		GameBox.left = kakuninPos2.x;
		GameBox.top = kakuninPos2.y;
		C_OffsetBox(GameBox.top, GameBox.left, GameBox.bottom, GameBox.right, 100 * 2.2f, 50 * 1.2f);

		//���� (�}�E�X�ƃ��g���C�Ƃ̃{�b�N�X)
		retry_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
			RetryeBox.top, RetryeBox.left, RetryeBox.bottom, RetryeBox.right);
		//���� (�}�E�X�ƃQ�[���ւƂ̃{�b�N�X)
		game_check = C_Hitcheck(MouseBox.top, MouseBox.left, MouseBox.bottom, MouseBox.right,
			GameBox.top, GameBox.left, GameBox.bottom, GameBox.right);

		//����
		if (retry_check)
		{
			tutorial_retry_[0] = true;
			tutorial_retry_[1] = false;
			hit = true;

			timer1++;
			timer2 = 0;

			framePos.x = kakuninPos.x;
			framePos.y = kakuninPos.y;
		}
		else if (game_check)
		{
			tutorial_retry_[0] = false;
			tutorial_retry_[1] = true;
			hit = true;

			timer1 = 0;
			timer2++;

			framePos.x = kakuninPos2.x;
			framePos.y = kakuninPos2.y;
		}
		else
		{
			tutorial_retry_[0] = false;
			tutorial_retry_[1] = false;
			timer1 = 0;
			timer2 = 0;

			hit = false;
		}

		GamePad& gamePad = Input::Instance().GetGamePad();
		switch (tutorial_state)
		{
		case PLAYER_MOVE:
			if (explanation == 1)
			{
				// ���@�ړ�(WASD)
				explaining = false;
				if (gamePad.GetButtonDown() & GamePad::BTN_UP)		wasd_[0] = true;
				if (gamePad.GetButtonDown() & GamePad::BTN_LEFT)	wasd_[1] = true;
				if (gamePad.GetButtonDown() & GamePad::BTN_DOWN)	wasd_[2] = true;
				if (gamePad.GetButtonDown() & GamePad::BTN_RIGHT)	wasd_[3] = true;

				// �W�����v(Space)
				if (gamePad.GetButtonDown() & GamePad::BTN_SPACE) jump_ = true;
				// �_�b�V��(WASD + Shift)
				if ((gamePad.GetButton() & GamePad::BTN_UP || gamePad.GetButton() & GamePad::BTN_LEFT || gamePad.GetButton() & GamePad::BTN_DOWN || gamePad.GetButton() & GamePad::BTN_RIGHT) && gamePad.GetButton() & GamePad::BTN_SHIFT) dash_ = true;
				// ���_�ړ�(�}�E�X)
				if (mouse.GetPositionX() != mouse.GetOldPositionX() || mouse.GetPositionY() != mouse.GetOldPositionY()) camera_move_ = true;



				if (wasd_[0] && wasd_[1] && wasd_[2] && wasd_[3] && jump_ && dash_ && camera_move_)
				{
					if (check_timer > 60)
					{
						End_of_explanation(elapsedTime);
						//enemy_Arrangement->enemy_produce(Enemy_Arrangement::csv_file_num::TUTORIAL_NORMAL);
						check_timer = 0;
					}
					else
					{
						check_timer++;
					}
				}

			}
			else
			{
				explaining = true;
			}
			break;
		case ATTACK_MOVE:
			if (explanation == 2)
			{
				explaining = false;
				if (mouse.GetButtonDown() & Mouse::BTN_RIGHT)	attack_[0] = true;
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT)	attack_[1] = true;

				if (attack_[0] && attack_[1])
				{
					if (check_timer > 60 && player->GetModel()->IsPlayAnimation() == false) 
					{
						End_of_explanation(elapsedTime);
						check_timer = 0;
					}
					else
					{
						check_timer++;
					}
				}
			}
			else
			{
				explaining = true;
			}
			break;
		case END:
			// �V�[���ύX
			if (explanation == 7)
			{
				// �͂�
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT && tutorial_retry_[0])
				{
					AudioManager::Instance().GetAudio(Audio_INDEX::SE_CLICK)->Play(false);
					explanation = 0; // �`���[�g���A����������x(�����������炢��)
					tutorial_state = 0;

					for (int i = 0; i < 2; i++)
					{
						attack_[i] = false;
					}
					
					for (int i = 0; i < 4; i++)
					{
						wasd_[i] = false;
					}

					jump_ = false;
					dash_ = false;
					camera_move_ = false;
				}

				// ������
				if (mouse.GetButtonDown() & Mouse::BTN_LEFT && tutorial_retry_[1])
				{
					AudioManager::Instance().GetAudio(Audio_INDEX::SE_CLICK)->Play(false);
					is_do_tutorial = false;
					explaining = false;

				}
			}
			else
			{
				explaining = true;
			}
			break;
		default:
			break;
		}
	}
	else if (countdown->NowCountDown() == false)
	{
		GameSystem::Instance().Update(elapsedTime);
	}
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

	if (is_do_tutorial == false)
	{
		countdown->SpriteRender(dc, { 960,396 }, { 1,1 });
	}

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
			player->par.health * 32, 64,
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


		//�X�^�~�i�t���[��
		StressBarFrame->Render2(dc,
			20, 95,
			1.0f, 1.0f,
			0, 0,
			512, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//�X�^�~�i
		StressBar->Render2(dc,
			20, 95,
			1.0f, 1.0f,
			0, 0,
			player->stamina, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//�X�^�~�i�A�C�R��
		StressIcon->Render2(dc,
			26, 103,
			1.0f, 1.0f,
			0, 0,
			48, 49,
			0, 0,
			0,
			1, 1, 1, 1);

#if 0
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
#endif

		//�~�j�}�b�v
		Minimap->Render2(dc,
			1600, 10,
			1.0f, 1.0f,
			0, 0,
			248, 248,
			0, 0,
			0,
			1, 1, 1, 1);

		Minimap_Brave_angle = DirectX::XMConvertToRadians(brave_timer_ * 0.0214f);

		//�~�j�}�b�v�̗E�҃A�C�R��
		Minimap_Brave->Render2(dc,
			1600 + 124, 10 + 124,
			1.0f, 1.0f,
			0, 0,
			248, 248,
			124, 124,
			Minimap_Brave_angle,
			1, 1, 1, 0.8f);

		Minimap_Player_angle = DirectX::XMConvertToRadians((175 + player->GetPosition().z) * 0.0648f); // (�X�^�[�g�n�_��-175�Ȃ̂Ō��_�ɖ߂����߂�+175 + �i�񂾋���) * 0.0648f��]������

		//�~�j�}�b�v�̃v���C���[�A�C�R��
		Minimap_Player->Render2(dc,
			1600 + 124, 10 + 124,
			1.0f, 1.0f,
			0, 0,
			248, 248,
			124, 124,
			Minimap_Player_angle,
			1, 1, 1, 0.8f);

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
			CastleHP, 64,
			0, 0,
			0,
			1, 1, 1, 1);

		//��A�C�R��
		CastleIcon->Render2(dc,
			1610, 285,
			1.0f, 1.0f,
			0, 0,
			43, 44,
			0, 0,
			0,
			1, 1, 1, 1);

		//�U���X���b�g1
		AttackSlot1->Render2(dc,
			attackPosx1, attackPosY1,
			1.0f * AttackSlotScale1, 1.0f * AttackSlotScale1,
			0, 0,
			153, 148,
			0, 0,
			0,
			1, 1, 1, 1);

		
		//�U���X���b�g2
		AttackSlot2->Render2(dc,
			attackPosx2, attackPosY2,
			1.0f * AttackSlotScale2, 1.0f * AttackSlotScale2,
			0, 0,
			153, 148,
			0, 0,
			0,
			1, 1, 1, 1);

		//�U���X���b�g3
		AttackSlot3->Render2(dc,
			attackPosx3, attackPosY3,
			1.0f * AttackSlotScale3, 1.0f * AttackSlotScale3,
			0, 0,
			153, 148,
			0, 0,
			0,
			1, 1, 1, 1);

		//�U���I��
		//AttackSelect->Render2(dc,
		//	150, 850,
		//	1.2f, 1.2f,
		//	0, 0,
		//	160, 160,
		//	0, 0,
		//	0,
		//	1, 1, 1, 1);
	}

	// �`���[�g���A��
	if (is_do_tutorial)
	{
		float spr_Width{ 1920.0f };
		float spr_Height{ 1080.0f };

		switch (explanation)
		{
		case 0:
			// ������
			spr_explanation_[0]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)
			break;

		case 1:
			// ������
			spr_explanation_[1]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)

			if (wasd_[0] && wasd_[1] && wasd_[2] && wasd_[3])
			{
				spr_check_1_[0]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}
			if (jump_)
			{
				spr_check_1_[1]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}
			if (dash_)
			{
				spr_check_1_[2]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}
			if (camera_move_)
			{
				spr_check_1_[3]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}

			break;

		case 2:
			// ������
			spr_explanation_[2]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)

			if (attack_[0])
			{
				spr_check_2_[0]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}
			if (attack_[1])
			{
				spr_check_2_[1]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}

			break;

		case 3:
			// ������
			spr_explanation_[3]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)
			break;

		case 4:
			// ������
			spr_explanation_[4]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)
			break;

		case 5:
			// ������
			spr_explanation_[5]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)
			break;

		case 6:
			// ������
			spr_explanation_[7]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)
			break;

		case 7:
			// ������
			spr_explanation_[6]->Render2(dc,
				0, 0,						// �\���ʒu
				1.0f, 1.0f,									// �X�P�[��
				0, 0,										// �摜�؂蔲���ʒu
				spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
				0, 0,	// �摜��_
				angle,										// �p�x
				1, 1, 1, 1);								// �F���(r,g,b,a)

			if (timer1 / 32 % 2 || !retry_check)
			if (tutorial_retry_[0])
			{

				spr_choice_arrow_[0]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}
			if (timer2 / 32 % 2 || !game_check)
			if (tutorial_retry_[1])
			{

				spr_choice_arrow_[1]->Render2(dc,
					0, 0,						// �\���ʒu
					1.0f, 1.0f,									// �X�P�[��
					0, 0,										// �摜�؂蔲���ʒu
					spr_Width, spr_Height,				// �摜�؂蔲���T�C�Y
					0, 0,	// �摜��_
					angle,										// �p�x
					1, 1, 1, 1);								// �F���(r,g,b,a)
			}

			//�}�E�X�J�[�\��
			spr_mouseCursor->Render(dc,
				mousePos.x, mousePos.y, 64, 64,
				0, 0, 64, 64,
				0,
				1, 1, 1, 1);
			break;
		}
	}

	//�t�F�[�h�A�E�g
	if (is_fadeSprite)
		spr_fadeOut->Render(dc,
			0, 0, 1920, 1080,
			0, 0, 1920, 1080,
			0,
			1, 1, 1, 1 - (1 - fade_timer / 120));

	pause->SpriteRender(dc);
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

	stage_num = STAGENUM::N_STAGE1_VOLCANO;

	attackPosY1 = 850;
	attackPosY2 = 850;
	attackPosY3 = 850;

	CastleHP = 256;

	fade_timer = 0;

	if (is_do_tutorial == false) explaining = false; // �`���[�g���A�������Ȃ��Ȃ�ŏ����瓮����悤��

	//�`���[�g���A���̂͂��A�������m�F
	tutorial_retry_[0] = false;
	tutorial_retry_[1] = false;

	kakuninPos.x = 1115;
	kakuninPos.y = 540;
	kakuninPos2.x = 1115;
	kakuninPos2.y = 605;
	framePos.x = kakuninPos.x;
	framePos.y = kakuninPos.y;

	AudioManager::Instance().GetAudio(Audio_INDEX::BGM_TITLE)->Stop();
}


void Game::Load()
{
	pause		= std::make_unique<Pause>(this);
	countdown = std::make_unique<CountDown>();

	// �v���C���[������
	player = new Player();
	player->ResetTransform();

#ifdef _DEBUG
	//player->SetPosition(DirectX::XMFLOAT3(0, 0, kStage4_Start_Position));
	player->SetPosition(DirectX::XMFLOAT3(0, 0, -155.0f));
#else
	player->SetPosition(DirectX::XMFLOAT3(0, 0, -155.0f));
#endif

	enemy_Arrangement = new Enemy_Arrangement();
	enemy_Arrangement->enemy_produce();

	black_band = std::make_unique<Sprite>();

	StageManager::Instance().AddStage(new Stage1_Volcano());
	StageManager::Instance().AddStage(new Stage2_Desert());
	StageManager::Instance().AddStage(new Stage3_Volcano());
	StageManager::Instance().AddStage(new Stage4_Volcano());
	StageManager::Instance().AddStage(new All_Stage());
	StageManager::Instance().AddStage(new castle());
	StageManager::Instance().AddStage(new Sky());

	//UI������
	//UI�B
#if 1
	HpBar					= std::make_unique<Sprite>("Data/Sprite/ui/G_HP.png");
	HpBarFrame				= std::make_unique<Sprite>("Data/Sprite/ui/HP����.png");
	HpIcon					= std::make_unique<Sprite>("Data/Sprite/ui/I_HP.png");
	StressBar				= std::make_unique<Sprite>("Data/Sprite/ui/�X�g���X.png");
	StressBarFrame			= std::make_unique<Sprite>("Data/Sprite/ui/�X�g���X����.png");
	StressIcon				= std::make_unique<Sprite>("Data/Sprite/ui/I_box.png");
	//GoldBar				= std::make_unique<Sprite>("Data/Sprite/G_gold.png");
	//GoldIcon				= std::make_unique<Sprite>("Data/Sprite/I_gold.png");
	Minimap					= std::make_unique<Sprite>("Data/Sprite/ui/map.png");
	Castlebar				= std::make_unique<Sprite>("Data/Sprite/ui/��.png");
	CastlebarFrame			= std::make_unique<Sprite>("Data/Sprite/ui/�鍕��.png");
	CastleIcon				= std::make_unique<Sprite>("Data/Sprite/ui/I_castle.png");
	AttackSlot1				= std::make_unique<Sprite>("Data/Sprite/ui/AT1.png");
	AttackSlot2				= std::make_unique<Sprite>("Data/Sprite/ui/AT2.png");
	AttackSlot3				= std::make_unique<Sprite>("Data/Sprite/ui/AT3.png");
	AttackSelect			= std::make_unique<Sprite>("Data/Sprite/ui/attack_waku_R.png");
	Minimap_Brave			= std::make_unique<Sprite>("Data/Sprite/ui/Brave.png");
	Minimap_Player			= std::make_unique<Sprite>("Data/Sprite/ui/Player.png");
#endif

	// �`���[�g���A��
#if 1
	if (is_do_tutorial)
	{

		spr_explanation_[0] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial2.png");
		spr_explanation_[1] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial3.png");
		spr_explanation_[2] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial4.png");
		spr_explanation_[3] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial5.png");
		spr_explanation_[4] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial6.png");
		spr_explanation_[5] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial7.png");
		spr_explanation_[6] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial8.png");
		spr_explanation_[7] = std::make_unique<Sprite>("Data/Sprite/tutorial/Tutorial6�ɂ��ė~����.png");

		spr_check_1_[0] = std::make_unique<Sprite>("Data/Sprite/tutorial/Check1_1.png");
		spr_check_1_[1] = std::make_unique<Sprite>("Data/Sprite/tutorial/Check1_2.png");
		spr_check_1_[2] = std::make_unique<Sprite>("Data/Sprite/tutorial/Check1_3.png");
		spr_check_1_[3] = std::make_unique<Sprite>("Data/Sprite/tutorial/Check1_4.png");

		spr_check_2_[0] = std::make_unique<Sprite>("Data/Sprite/tutorial/Check2_1.png");
		spr_check_2_[1] = std::make_unique<Sprite>("Data/Sprite/tutorial/Check2_2.png");

		spr_choice_arrow_[0] = std::make_unique<Sprite>("Data/Sprite/tutorial/�͂��̎��̖��.png");
		spr_choice_arrow_[1] = std::make_unique<Sprite>("Data/Sprite/tutorial/�������̎��̖��.png");


		spr_mouseCursor = std::make_unique<Sprite>("Data/Sprite/cursor.png");
	}
#endif

	//�Q�[���I�[�o�[���t�F�[�h�A�E�g
	spr_fadeOut = std::make_unique<Sprite>("Data/Sprite/back.png");
}


void Game::ImGui()
{
	ImGui::Text("scene : Game");

	ImGui::Spacing();

	ImGui::Text("now_time : %.1f", GameSystem::Instance().NowTime());

	ImGui::Text("stage_num : %d", stage_num);

	if (ImGui::CollapsingHeader("tutorial", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("tutorial_state : %d", tutorial_state);
	}

	if (ImGui::CollapsingHeader("player", ImGuiTreeNodeFlags_DefaultOpen))
	{
		DirectX::XMFLOAT3 pos = player->GetPosition();
		ImGui::Text("player pos %.1f, %.1f, %.1f", pos.x, pos.y, pos.z);

		float length;
		DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&player->GetVelocity())));
		ImGui::Text("velo length : %.1f", length);

		DirectX::XMFLOAT3 velo = player->GetVelocity();
		ImGui::Text("player velo %.1f, %.1f, %.1f", velo.x, velo.y, velo.z);

		player->DrawDebugGUI();
	}

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
	// number����
	switch (stage_num)
	{
	case N_STAGE1_VOLCANO:
		if (player->GetPosition().z >= kStage2_Start_Position) stage_num = N_STAGE2_DESERT; // 1240 > p >= 565
		break;

	case N_STAGE2_DESERT:
		if (player->GetPosition().z >= kStage3_Start_Position) stage_num = N_STAGE3_;	  // 1910 > p >= 1240
		break;

	case N_STAGE3_:
		if (player->GetPosition().z >= kStage4_Start_Position) stage_num = N_STAGE4_;							  // p >= 1920
		break;

	//case N_STAGE4_:
	//	break;

	}

	// �ړ�����

	switch (stage_num)
	{
	case N_STAGE2_DESERT:
		if (player->GetPosition().z < kStage2_Start_Position) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, kStage2_Start_Position });
		break;
		
	case N_STAGE3_:
		if (player->GetPosition().z < kStage3_Start_Position) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, kStage3_Start_Position });
		break;
		
	case N_STAGE4_:
		if (player->GetPosition().z < kStage4_Start_Position) player->SetPosition({ player->GetPosition().x, player->GetPosition().y, kStage4_Start_Position });
		break;

	}
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
		3200.0f
	);

	//�J�����R���g���[���[������
	cameraController = new CameraController();

	cameraController->init(player->GetPosition(), player->GetPosition());
}


void Game::ChangeScene(float elapsedTime)
{
#if _DEBUG
	ChangeNextScene(new Result(), GamePad::BTN_A, false);
#endif

	if (CastleHP > 0.0f) return;

	// ���т̍X�V
	black_band_timer += 1.0f * elapsedTime;


	// ���т��~�肫������
	if (black_band_timer >= 1.4f)
	{

		result = Game_over2;
		// �c�莞�Ԃ��[���ɂȂ����ۃV�[���J�ڂ�����
		ChangeNextScene(new Result(), false);

		/*if(bgm_caution == false)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_NORMAL)->Stop();
		if (bgm_caution == true)AudioManager::Instance().GetAudio(Audio_INDEX::BGM_SPEED)->Stop();*/
	}
}


void Game::ClearedSpriteRender(ID3D11DeviceContext* dc)
{
	if (black_band_timer == 0.0f) return;

	// ����
	constexpr float scale = 300.0f;

	black_band->Render(dc, 0, 0, 1920, scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 0, 0, 0, 1);
	black_band->Render(dc, 0, 1080, 1920, -scale * pow(black_band_timer, 5), 0, 0, 0, 0, 0, 0, 0, 0, 1);
}


void Game::BGMStart()
{

	switch (stage_num)
	{
	case N_STAGE1_VOLCANO:
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE2)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE1)->Play(true);
		break;
	case N_STAGE2_DESERT:
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE1)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE2)->Play(true, 1.0f);
		break;
	case N_STAGE3_:
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE2)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE3)->Play(true);
		break;
	case N_STAGE4_:
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE3)->Stop();
		AudioManager::Instance().GetAudio(Audio_INDEX::BGM_STAGE4)->Play(true, 0.3f);
		break;
	default:
		break;
	}
}

void Game::End_of_explanation(float elapsedTime)
{
	//AudioManager::Instance().GetAudio(Audio_INDEX::SE_SUCCESS)->Stop();
	//AudioManager::Instance().GetAudio(Audio_INDEX::SE_SUCCESS)->Play(false);

	tutorial_state++;

	//player->ResetTransform();

	//CameraSet();

	if (explanation < 8) explanation++;
	explaining = true;
}
