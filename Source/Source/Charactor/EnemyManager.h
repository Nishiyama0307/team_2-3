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
	void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num = 0);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

	// �G�l�~�[�o�^
	void Register(Enemy* enemy, ENEMYTAG tag);

	// �G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// �G�l�~�[�擾
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	// �G�l�~�[�S�폜
	void Clear();

	// �G�l�~�[�폜
	void Remove(Enemy* enemy);

	void SortLengthSq(const DirectX::XMFLOAT3& playerPos);

private:
	// �G�l�~�[���m�̏Փ˔���
	void CollisionEnemyVsEnemies(int stage_num);

	std::vector<Enemy*>		enemies;
	std::vector<Enemy*>		removes;
};