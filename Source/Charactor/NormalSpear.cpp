#include "NormalSpear.h"
#include "EnemyManager.h"

#include "Input/Input.h"
NormalSpear::NormalSpear()
{
	model = new Model("Data/Model/spear/spear.mdl");

	enemy_tag = NORMAL_SPEAR;

	scale.x = scale.y = scale.z = 1.0f;

	model->PlayAnimation(0, true);

	// {hp, attack, money}
	par = { 10, 5, 5 };
}

NormalSpear::~NormalSpear()
{
	delete model;
	model = NULL;
}

void NormalSpear::Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
{
	// ���͏����X�V
	UpdateVelocity(elapsedTime, KIND::RARE_ENEMY);

	// �I�u�W�F�N�g�s����X�V
	UpdateTransform();

	// ���G���ԍX�V
	UpdateInvicibleTimer(elapsedTime);

	// ���f���s��X�V
	model->UpdateTransform(transform);

	// ���f���A�j���[�V�����X�V����
	model->UpdateAnimation(elapsedTime);

}

void NormalSpear::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

DirectX::XMFLOAT3 NormalSpear::inhaled()
{
	constexpr float STAGE_1_SCALING_VALUE = 0.5f;

	switch (1 /* stage */)
	{
	case 1:
		Is_inhaling = true;
		EnemyManager::Instance().Remove(this);
		//par.scaling_value = { STAGE_1_SCALING_VALUE * scale.x, STAGE_1_SCALING_VALUE * scale.y, STAGE_1_SCALING_VALUE * scale.z };
		//return par.scaling_value;
		return { 0,0,0 };
		break;
	default:
		break;
	}
}

void NormalSpear::SetInhaleParameter()
// �z�����ݗp�̃p�����[�^�ݒ�
{

	float total_scale_value = (scale.x + scale.y + scale.z);

	// �����蔻��̔��a
	//radius = total_scale_value / 3;

	// �z�����߂�悤�ɂȂ�Œ���̃X�P�[��
	//par.enough_total_scale_value = total_scale_value * 0.87f;

	// �����ł���悤�ɂȂ�Œ���̃X�P�[��
	// �����肦�Ȃ����l�����Ă���
	//par.can_be_digestion_total_scale_value = FLT_MAX;
}