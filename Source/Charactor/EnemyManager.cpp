#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Collision.h"

void EnemyManager::Update(float elapsedTime)
{
	for (auto& enemy : enemies)
	{
		enemy->Update(elapsedTime);
	}

	for (auto& enemy : enemies)
	{
		enemy->SetHitPosition(enemy->GetPosition());
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
	CollisionEnemyVsEnemies();
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

// エネミー登録
void EnemyManager::Register(Enemy* enemy, int tag)
{
	enemy->SetInhaleParameter();
	enemies.emplace_back(enemy);
	enemy->enemy_tag = tag;
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


// 当たり判定の位置更新
void EnemyManager::HitPositionUpdate()
{
	for (auto& enemy : enemies)
	{
		enemy->HitPositionUpdate();
	}
}

// エネミー同士の衝突判定
void EnemyManager::CollisionEnemyVsEnemies()
{
	size_t enemyCount = enemies.size();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemyA = enemies.at(i);
		for (int j = i + 1; j < enemyCount; ++j)
		{
			Enemy* enemyB = enemies.at(j);

			DirectX::XMFLOAT3 outPosition;
			if (Collision3D::IntersectCylinderVsCylinder(
				enemyA->GetHitPosition(),
				enemyA->GetRadius(),
				enemyA->GetHeight(),
				enemyB->GetHitPosition(),
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
