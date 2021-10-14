#include "BombEnemy.h"
#include "EnemyManager.h"


BombEnemy::BombEnemy()
{
	model = new Model("Data/Model/kai_bakudan.mdl");

	enemy_tag = BOMB;

	scale.x = scale.y = scale.z = 1.0f;

	// ���A�����ݒ�
	height = 2.0f;
}

BombEnemy::~BombEnemy()
{
	delete model;
	model = NULL;
}

void BombEnemy::Update(float elapsedTime)
{
	// ���͏����X�V
	UpdateVelocity(elapsedTime, KIND::BOMB_ENEMY);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���G���ԍX�V
	UpdateInvicibleTimer(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);
}

void BombEnemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

DirectX::XMFLOAT3 BombEnemy::inhaled()
{
	constexpr float STAGE_1_SCALING_VALUE = -0.1f;

	switch ( 1 /* stage */)
	{
	case 1:
		Is_inhaling = true;
		EnemyManager::Instance().Remove(this);
		par.scaling_value = { STAGE_1_SCALING_VALUE * scale.x, STAGE_1_SCALING_VALUE * scale.y, STAGE_1_SCALING_VALUE * scale.z };
		return par.scaling_value;
		break;
	default:
		break;
	}
}

void BombEnemy::SetInhaleParameter()
// �z�����ݗp�̃p�����[�^�ݒ�
{
	float total_scale_value = (scale.x + scale.y + scale.z);

	// �����蔻��̔��a
	radius = total_scale_value / 3;

	// �z�����߂�悤�ɂȂ�Œ���̃X�P�[��
	par.enough_total_scale_value = total_scale_value * 0.87f;

	// �����ł���悤�ɂȂ�Œ���̃X�P�[��
	par.can_be_digestion_total_scale_value = total_scale_value * 2.0f;
}

// ���S�����Ƃ��ɌĂ΂��
void BombEnemy::OnDead()
{
	EnemyManager::Instance().Remove(this);
}