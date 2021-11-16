#include "StrongSpear.h"
#include "EnemyManager.h"

#include "Input/Input.h"
StrongSpear::StrongSpear()
{
	model = new Model("Data/Model/strongspear/strongspear.mdl");

	enemy_tag = STRONG_SPEAR;

	model->PlayAnimation(ANIMINDEX::IDLE, true);

	// {hp, attack, money}
	par = { 10, 5, 5 };
}

StrongSpear::~StrongSpear()
{
	delete model;
	model = NULL;
}

void StrongSpear::Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num)
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

void StrongSpear::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}
