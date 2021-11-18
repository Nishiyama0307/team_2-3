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
	if (stage_num != this->stage_num && this->stage_num == 1 && isGround == false) // 例外的に
	{
		// 速力処理更新
		UpdateVelocity(elapsedTime, KIND::RARE_ENEMY, playerPos);
	}
	else
	{
		// 速力処理更新
		UpdateVelocity(elapsedTime, KIND::RARE_ENEMY, playerPos);

		// オブジェクト行列を更新
		UpdateTransform();

		// 無敵時間更新
		UpdateInvicibleTimer(elapsedTime);

		// モデル行列更新
		model->UpdateTransform(transform);

		// モデルアニメーション更新処理
		model->UpdateAnimation(elapsedTime);
	}
}

void NormalSpear::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}