

#include "inhale.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "common.h"
#include "easy_math.h"
#include "Player.h"
#include "EnemyManager.h"
#include "collision.h"
#include "gameSystem.h"
#include "CameraController.h"

#include "audioManager.h"



Inhale::Inhale(Player* player_) : player(player_)
{
	// �X�P�[���ɍ��킹���|�W�V����
	nozzle.position = float3SUM(player->GetPosition(), float3Scaling(player->GetFront(), 3));

}

void Inhale::Update(float elapsedTime)
{
	UpdateNozzlePosition();

	Collision();
}


void Inhale::DebugRender()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

	// �Փ˔���p�̃f�o�b�O����`��

	if (is_during_inhale == false)	// �F�ŕ\��
		debugRenderer->DrawSphere(nozzle.position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));

	else							// �ԐF�ŕ\��
		debugRenderer->DrawSphere(nozzle.position, radius, DirectX::XMFLOAT4(1, 0, 0, 1));
}

void Inhale::InputInhale()
{
    is_during_inhale = false;

	if (Input::Instance().GetGamePad().GetButton() & GamePad::BTN_SPACE /*Z key*/)
	{
		is_during_inhale = true;
	}
}


void Inhale::UpdateNozzlePosition()
{
	// �m�Y�����{�̂���ǂꂮ�炢��ɂ���̂����v�Z
	float scale_factor = 1;
	DirectX::XMFLOAT3 pos = player->GetPosition();
	nozzle.position = float3SUM({ pos.x, pos.y - 1 * 0.7f, pos.z }, float3Scaling(player->GetFront(), scale_factor + scale_factor * 3.5f));

	// �����蔻��̑傫�����ύX����
	radius = 1;
}


void Inhale::Collision()
{
	if (is_during_inhale == false) return;
	// �z�����ݓ��쒆�Ȃ���s


	EnemyManager& enemyManager = EnemyManager::Instance();

	int enemyCount = enemyManager.GetEnemyCount();

	for (int j = 0; j < enemyCount; ++j)
	{
		Enemy* enemy = enemyManager.GetEnemy(j);

		// �Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::BallVsBallAndExtrusion/*collision::IntersectSqhereVsXYCircle*/(
			nozzle.position,
			radius,
			enemy->GetPosition(),
			enemy->GetRadius(),
			/*enemy->GetHeight(),*/
			outPosition
		))
		{
			// �z�����߂�ł����ɒB���Ă��Ȃ���΁@�z�����܂��e���Ԃ�
			if (3 < enemy->EnoughTotalScaleValue())
			{
				KnockBack(player->GetPosition(), enemy->GetPosition());

				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Play(false);
				continue;
			}


			// �X�P�[���̉��Z�Ƌz�����ݏ����̊m��
			DirectX::XMFLOAT3 add_scale = enemy->inhaled();

			float factor = 1.0f;
			if (3 >= enemy->CanBeDigestionTotalScaleValue()) factor *= -1.0f;

			// TODO: �X�R�A���Z�̉�����, �ύX����
			//GameSystem::Instance().AddScore(static_cast<int>(sum_add_scale * 10.0f));
			
			GameSystem::Instance().SetHitStop();



			if (enemy->enemy_tag == ENEMYTAG::NORMAL)
			{
				// Normal�̋z�����݉�
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_NORMAL)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_NORMAL)->Play(false);
			}
			if (enemy->enemy_tag == ENEMYTAG::BOMB)
			{
				// Bomb�̋z�����݉�
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_BOMB)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_BOMB)->Play(false);

			}
			if (enemy->enemy_tag == ENEMYTAG::RARE)
			{
				// Rare�̋z�����݉�
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_NORMAL)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_NORMAL)->Play(false);
			}



			break;
		}
	}
}



void Inhale::KnockBack(DirectX::XMFLOAT3 pos_a, DirectX::XMFLOAT3 pos_b)
{
	DirectX::XMFLOAT3 impact = {};
	DirectX::XMStoreFloat3(&impact, DirectX::XMVectorScale(DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pos_a), DirectX::XMLoadFloat3(&pos_b))), 25.0f));

	player->AddImpact(impact);

	//CameraController::Instance()->SetCameraShake();
}