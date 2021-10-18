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

	// �j������
	// ��enemies�͈̔�for������erase()����ƕs����������Ă��܂����߁A
	// �@�X�V�������I�������ɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g���폜����B
	for (Enemy* enemy : removes)
	{
		// std::vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜���Ȃ���΂Ȃ�Ȃ�
		std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);

		if (it != enemies.end())
		{
			enemies.erase(it);
		}

		// �G�l�~�[�̔j������
		delete enemy;
	}
	// �j�����X�g���N���A
	removes.clear();

	// �G�l�~�[���m�̏Փ˔���
	CollisionEnemyVsEnemies();
}

#include <algorithm>
void EnemyManager::SortLengthSq(const DirectX::XMFLOAT3& playerPos)
{
	//�e�G�Ƀv���C���[�Ƃ̋�����2���ێ�����
	for (auto& enemy : enemies)
	{
		float vx = playerPos.x - enemy->GetPosition().x;
		float vy = playerPos.y - enemy->GetPosition().y;
		float vz = playerPos.z - enemy->GetPosition().z;
		enemy->SetLengthSq(vx * vx + vy * vy + vz * vz);
	}
	std::sort(enemies.begin(), enemies.end(),
		[](Enemy* a, Enemy* b)->int { return a->GetLengthSq() < b->GetLengthSq(); });

	//�v���C���[�Ƃ̋�����2��Ń\�[�g����
	//enemies��LengthSq�Ń\�[�g����
}

void EnemyManager::Render(ID3D11DeviceContext* context, Shader* shader)
{
	for (auto& enemy : enemies)
	{
		enemy->Render(context, shader);
	}
}

// �f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive()
{
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugPrimitive();
	}
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy, int tag)
{
	enemy->SetInhaleParameter();
	enemies.emplace_back(enemy);
	enemy->enemy_tag = tag;
}
void EnemyManager::Clear()
{
	// �G�l�~�[�I����
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

// �G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy)
{
	removes.emplace_back(enemy);
}


// �����蔻��̈ʒu�X�V
void EnemyManager::HitPositionUpdate()
{
	for (auto& enemy : enemies)
	{
		enemy->HitPositionUpdate();
	}
}

// �G�l�~�[���m�̏Փ˔���
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
				//�����o����̈ʒu�ݒ�
				enemyB->SetPosition(outPosition);
			}
			
		}
	}
}
