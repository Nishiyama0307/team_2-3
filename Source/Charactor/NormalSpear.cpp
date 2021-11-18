#include "NormalSpear.h"
#include "EnemyManager.h"

#include "Input/Input.h"
NormalSpear::NormalSpear()
{
	model = new Model("Data/Model/spear/spear.mdl");

	enemy_tag = NORMAL_SPEAR;

	model->PlayAnimation(ANIMINDEX::IDLE, true);

	// {hp, attack, money}
	par = { 10, 5, 5 };
}

NormalSpear::~NormalSpear()
{
	delete model;
	model = NULL;
}

void NormalSpear::Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num)
{
	if (stage_num != this->stage_num && this->stage_num == 1 && isGround == false) // ��O�I��
	{
		// ���͏����X�V
		UpdateVelocity(elapsedTime, KIND::RARE_ENEMY, playerPos);
	}
	else
	{
		// ���͏����X�V
		UpdateVelocity(elapsedTime, KIND::RARE_ENEMY, playerPos);

		// �I�u�W�F�N�g�s����X�V
		UpdateTransform();

		// ���G���ԍX�V
		UpdateInvicibleTimer(elapsedTime);

		// ���f���s��X�V
		model->UpdateTransform(transform);

		// ���f���A�j���[�V�����X�V����
		model->UpdateAnimation(elapsedTime);
	}
}

void NormalSpear::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}