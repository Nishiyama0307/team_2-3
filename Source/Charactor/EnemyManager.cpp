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
			if (enemy->stage_num == 0 && enemy->GetPosition().y <= -15) enemy->ApplyDamage(enemy->par.health, 3, 0);//����3��KIND::RARE_ENEMY�ł�
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
			if (enemy1->is_tracking_) // 1�l��player�ɋC�Â�����
			{
				DirectX::XMFLOAT3 Benchmark = enemy1->GetPosition(); // ���position�������


				for (auto& enemy : enemies) // �S�Ă̓G��
				{
					if (enemy->is_tracking_) continue; // �ǔ����Ȃ�continue
					// target�X�V�̂��߃R�����g�A�E�g
					if (enemy->is_group_behavior_) continue; // �c�̍s�����Ȃ�continue
					if (enemy->is_dead_) continue;
					if (stage_num != enemy->stage_num) continue;

					float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Benchmark), DirectX::XMLoadFloat3(&enemy->GetPosition()))));

					if (distance < range) // �͈͓��Ȃ�
					{
						enemy->is_group_behavior_ = true; // �c�̍s������
						if (enemy->model->GetIndex() != 2) enemy->model->PlayAnimation(ANIMINDEX::RUN, true); // ���郂�[�V����
					}
				}
			}

			if (enemy1->is_group_behavior_) // �c�̍s�����Ă�����
			{
				DirectX::XMFLOAT3 Benchmark = enemy1->GetPosition(); // ���position�������
				int i = 0;

				for (auto& enemy : enemies) // �S�Ă̓G��
				{
					if (enemy->is_group_behavior_) continue;
					if (enemy->is_dead_) continue;

					if (enemy->is_tracking_) // �ǐՒ��̐l�����邩
					{
						float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&Benchmark), DirectX::XMLoadFloat3(&enemy->GetPosition()))));

						if (distance < range) // �ǐՒ��̐l�����āA����ɒc�̍s���͈͓̔��Ȃ�
						{
							enemy1->target = enemy->GetPosition(); // �^�[�Q�b�g�͒ǐՒ��̐l�ɍX�V��������
							i++;
							if (i > 0) break; // 1�l�ł�������c�̍s�����p������
						}
					}
				}

				if (i == 0) // �͈͓��ŒǐՒ��̐l�����Ȃ����
				{
					enemy1->is_group_behavior_ = false; // �c�̍s������߂�
					enemy1->model->PlayAnimation(ANIMINDEX::IDLE, true);
				}
			}
		}
		else if(enemy1->is_dead_ && stage_num == enemy1->stage_num ) // ���񂾂�O�̂���bool�ϐ���false�ɂ��Ă���
		{
			enemy1->is_tracking_ = false;
			enemy1->is_detection_ = false;
			enemy1->is_group_behavior_ = false; // �c�̍s������߂�
			enemy1->is_attacking_ = false;
			enemy1->is_walking = false;
		}
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
	CollisionEnemyVsEnemies(stage_num);
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

// �f�o�b�O�pGUI�`��
void EnemyManager::DrawDebugGUI()
{
	int i = 0;
	for (auto& enemy : enemies)
	{
		enemy->DrawDebugGUI(i);
		i++;
	}
}

// �G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy, ENEMYTAG tag)
{
	// ��_�̐ݒ�
	enemy->start_position = enemy->GetPosition();
	// �z�u��Ɉ�x�����X�V���邱�ƂŌ��_����ړ�������
	enemy->UpdateTransform();
	enemy->model->UpdateTransform(enemy->GetTransform());
	// �ǂ̎�ނ̓G��
	enemy->enemy_tag = tag;
	// �o�^
	enemies.emplace_back(enemy);
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

// �G�l�~�[���m�̏Փ˔���
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
				//�����o����̈ʒu�ݒ�
				enemyB->SetPosition(outPosition);
			}

		}
	}
}
