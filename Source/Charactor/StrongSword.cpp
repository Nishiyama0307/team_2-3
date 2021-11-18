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

void StrongSword::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}