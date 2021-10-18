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

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �G�l�~�[�o�^
	void Register(Enemy* enemy, int tag);

	// �G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

	// �G�l�~�[�擾
	Enemy* GetEnemy(int index) { return enemies.at(index); }

	// �G�l�~�[�S�폜
	void Clear();

	// �G�l�~�[�폜
	void Remove(Enemy* enemy);

	// �����蔻��̈ʒu�X�V
	void HitPositionUpdate();

	void SortLengthSq(const DirectX::XMFLOAT3& playerPos);

private:
	// �G�l�~�[���m�̏Փ˔���
	void CollisionEnemyVsEnemies();

	std::vector<Enemy*>		enemies;
	std::vector<Enemy*>		removes;
};