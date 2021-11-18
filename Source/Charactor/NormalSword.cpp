#include "NormalSword.h"
#include "EnemyManager.h"

#include "Input/Input.h"

NormalSword::NormalSword()
{
	model = new Model("Data/Model/sword/sword.mdl");

	enemy_tag = NORMAL_SWORD;

	model->PlayAnimation(ANIMINDEX::IDLE, true);

	// {hp, attack, money}
	par = {10, 5, 5};
}

NormalSword::~NormalSword()
{
	delete model;
	model = NULL;
}

void NormalSword::Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num)
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


void NormalSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}