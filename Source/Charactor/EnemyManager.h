#pragma once

#include <vector>
#include "Enemy.h"

class EnemyManager
{
private:
	EnemyManager(){}
	~EnemyManager() {}

public:
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}

	void Update(float elapsedTime);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// エネミー登録
	void Register(Enemy* enemy, int tag);

	// エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	// エネミー全削除
	void Clear();

	// エネミー削除
	void Remove(Enemy* enemy);

	// 当たり判定の位置更新
	void HitPositionUpdate();

	void SortLengthSq(const DirectX::XMFLOAT3& playerPos);

private:
	// エネミー同士の衝突判定
	void CollisionEnemyVsEnemies();

	std::vector<Enemy*>		enemies;
	std::vector<Enemy*>		removes;
};