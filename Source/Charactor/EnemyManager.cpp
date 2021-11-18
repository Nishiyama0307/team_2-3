#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

#include "Charactor/Character.h"
void EnemyManager::Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num)
{

	for (auto& enemy : enemies)
	{
		if (stage_num > enemy->stage_num) enemy->Destroy();

		//if (enemy->is_dead_ == false)
		if (enemy->is_dead_ == false && stage_num == enemy->stage_num)
		{
			if (enemy->stage_num == 0 && enemy->GetPosition().y <= -15) enemy->ApplyDamage(enemy->par.health, 3, 0);//この3はKIND::RARE_ENEMYです
			enemy->UpdateMove(elapsedTime, playerPos);
		}
		if(enemy->is_dead_)
		enemy->attack = false;
	}
	for (auto& enemy : enemies)
	{
		if (stage_num == enemy->stage_num || 1 == enemy->stage_num) // 
		{
			enemy->Update(elapsedTime, playerPos, stage_num);
		}
	}

	constexpr int range = 50;

	for (auto& enemy1 : enemies)
	{
		//if (enemy1->is_dead_ == false)
		if (enemy1->is_dead_ == false && stage_num == enemy1->stage_num)
		{
			if (enemy1->is_tracking_) // 1人がplayerに気づいたら
			{
				DirectX::XMFLOAT3 Benchmark = enemy1->GetPosition(); // 基準のpositionを取って


				for (auto& enemy : enemies) // 全ての敵と
				{
					if (enemy->is_tracking_) continue; // 追尾中ならcontinue
					// target更新のためコメントアウト
					if (enemy->is_group_behavior_) continue; // 団体行動中ならcontinue
					if (enemy->is_dead_) continue;
					if (stage_num != enemy->stage_num) continue;

					float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Benchmark), DirectX::XMLoadFloat3(&enemy->GetPosition()))));

					if (distance < range) // 範囲内なら
					{
						enemy->is_group_behavior_ = true; // 団体行動する
						if (enemy->model->GetIndex() != 2) enemy->model->PlayAnimation(ANIMINDEX::RUN, true); // 走るモーション
					}
				}
			}

			if (enemy1->is_group_behavior_) // 団体行動していたら
			{
				DirectX::XMFLOAT3 Benchmark = enemy1->GetPosition(); // 基準のpositionを取って
				int i = 0;

				for (auto& enemy : enemies) // 全ての敵と
				{
					if (enemy->is_group_behavior_) continue;
					if (enemy->is_dead_) continue;

					if (enemy->is_tracking_) // 追跡中の人がいるか
					{
						float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Benchmark), DirectX::XMLoadFloat3(&enemy->GetPosition()))));

						if (distance < range) // 追跡中の人が居て、さらに団体行動の範囲内なら
						{
							enemy1->target = enemy->GetPosition(); // ターゲットは追跡中の人に更新し続ける
							i++;
							if (i > 0) break; // 1人でもいたら団体行動を継続する
						}
					}
				}

				if (i == 0) // 範囲内で追跡中の人がいなければ
				{
					enemy1->is_group_behavior_ = false; // 団体行動をやめる
					enemy1->model->PlayAnimation(ANIMINDEX::IDLE, true);
				}
			}
		}
		else if(enemy1->is_dead_ && stage_num == enemy1->stage_num ) // 死んだら念のためbool変数をfalseにしておく
		{
			enemy1->is_tracking_ = false;
			enemy1->is_detection_ = false;
			enemy1->is_group_behavior_ = false; // 団体行動をやめる
			enemy1->is_attacking_ = false;
			enemy1->is_walking = false;
		}
	}

	// 破棄処理
	// ※enemiesの範囲for文中でerase()すると不具合が発生してしまうため、
	// 　更新処理が終わった後に破棄リストに積まれたオブジェクトを削除する。
	for (Enemy* enemy : removes)
	{
		// std::vectorから要素を削除する場合はイテレーターで削除しなければならない
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// エネミーの破棄処理
		delete enemy;
	}
	// 破棄リストをクリア
	removes.clear();

	// エネミー同士の衝突判定
	CollisionEnemyVsEnemies(stage_num);
}

#include <algorithm>
void EnemyManager::SortLengthSq(const DirectX::XMFLOAT3& playerPos)
{
	//各敵にプレイヤーとの距離の2乗を保持する
	for (auto& enemy : enemies)
	{
		float vx = playerPos.x - enemy->GetPosition().x;
		float vy = playerPos.y - enemy->GetPosition().y;
		float vz = playerPos.z - enemy->GetPosition().z;
		enemy->SetLengthSq(vx * vx + vy * vy + vz * vz);
	}
	std::sort(enemies.begin(), enemies.end(),
		[](Enemy* a, Enemy* b)->int { return a->GetLengthSq() < b->GetLengthSq(); });

	//プレイヤーとの距離の2乗でソートする
	//enemiesをLengthSqでソートする
}

void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (auto& enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}

// デバッグプリミティブ描画
void EnemyManager::DrawDebugPrimitive()
{
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

// デバッグ用GUI描画
void EnemyManager::DrawDebugGUI()
{
	int i = 0;
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugGUI(i);
		i++;
	}
}

// エネミー登録
void EnemyManager::Register(Enemy* enemy, ENEMYTAG tag)
{
	// 基準点の設定
	enemy->start_position = enemy->GetPosition();
	// 配置後に一度だけ更新することで原点から移動させる
	enemy->UpdateTransform();
	enemy->model->UpdateTransform(enemy->GetTransform());
	// どの種類の敵か
	enemy->enemy_tag = tag;
	// 登録
	enemies.emplace_back(enemy);
}
void EnemyManager::Clear()
{
	// エネミー終了化
	for (Enemy* enemy : enemies)
	{
		//enemy->~Enemy();
		if (enemy != NULL)
		{
			enemies.clear();
			delete enemy;
		}
	}
}

// エネミー削除
void EnemyManager::Remove(Enemy* enemy)
{
	removes.emplace_back(enemy);
}

// エネミー同士の衝突判定
void EnemyManager::CollisionEnemyVsEnemies(int stage_num)
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		if (enemyA->stage_num != stage_num) continue;

		for (int j = i + 1; j < enemyCount; ++j)
		{
			Enemy* enemyB = enemies.at(j);
			if (enemyB->stage_num != stage_num) continue;

			DirectX::XMFLOAT3 outPosition;
			if (Collision3D::IntersectCylinderVsCylinder(
				enemyA->GetPosition(),
				enemyA->GetRadius(),
				enemyA->GetHeight(),
				enemyB->GetPosition(),
				enemyB->GetRadius(),
				enemyB->GetHeight(),
				outPosition
			))
			{
				//押し出し後の位置設定
				enemyB->SetPosition(outPosition);
			}

		}
	}
}
