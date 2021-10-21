

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
	// スケールに合わせたポジション
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

	// 衝突判定用のデバッグ球を描画

	if (is_during_inhale == false)	// 青色で表示
		debugRenderer->DrawSphere(nozzle.position, radius, DirectX::XMFLOAT4(0, 0, 1, 1));

	else							// 赤色で表示
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
	// ノズルが本体からどれぐらい先にあるのかを計算
	float scale_factor = 1;
	DirectX::XMFLOAT3 pos = player->GetPosition();
	nozzle.position = float3SUM({ pos.x, pos.y - 1 * 0.7f, pos.z }, float3Scaling(player->GetFront(), scale_factor + scale_factor * 3.5f));

	// 当たり判定の大きさも変更する
	radius = 1;
}


void Inhale::Collision()
{
	if (is_during_inhale == false) return;
	// 吸い込み動作中なら実行


	EnemyManager& enemyManager = EnemyManager::Instance();

	int enemyCount = enemyManager.GetEnemyCount();

	for (int j = 0; j < enemyCount; ++j)
	{
		Enemy* enemy = enemyManager.GetEnemy(j);

		// 衝突処理
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
			// 吸い込めるでかさに達していなければ　吸い込まず弾き返す
			if (3 < enemy->EnoughTotalScaleValue())
			{
				KnockBack(player->GetPosition(), enemy->GetPosition());

				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Play(false);
				continue;
			}


			// スケールの加算と吸い込み処理の確定
			DirectX::XMFLOAT3 add_scale = enemy->inhaled();

			float factor = 1.0f;
			if (3 >= enemy->CanBeDigestionTotalScaleValue()) factor *= -1.0f;

			// TODO: スコア加算の仮実装, 変更する
			//GameSystem::Instance().AddScore(static_cast<int>(sum_add_scale * 10.0f));
			
			GameSystem::Instance().SetHitStop();



			if (enemy->enemy_tag == ENEMYTAG::NORMAL)
			{
				// Normalの吸い込み音
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_NORMAL)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_NORMAL)->Play(false);
			}
			if (enemy->enemy_tag == ENEMYTAG::BOMB)
			{
				// Bombの吸い込み音
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_BOMB)->Stop();
				AudioManager::Instance().GetAudio(Audio_INDEX::SE_INHALE_BOMB)->Play(false);

			}
			if (enemy->enemy_tag == ENEMYTAG::RARE)
			{
				// Rareの吸い込み音
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