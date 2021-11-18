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


void NormalSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}