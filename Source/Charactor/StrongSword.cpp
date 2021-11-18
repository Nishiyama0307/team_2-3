#include "StrongSword.h"
#include "EnemyManager.h"

#include "Input/Input.h"
StrongSword::StrongSword()
{
	model = new Model("Data/Model/strongsword/strongsword.mdl");

	enemy_tag = STRONG_SWORD;

	model->PlayAnimation(ANIMINDEX::IDLE, true);

	// {hp, attack, money}
	par = { 10, 2 };
}

StrongSword::~StrongSword()
{
	delete model;
	model = NULL;
}

void StrongSword::Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num)
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

void StrongSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}