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
	// 速力処理更新
	UpdateVelocity(elapsedTime, KIND::RARE_ENEMY);

	// オブジェクト行列を更新
	UpdateTransform();

	// 無敵時間更新
	UpdateInvicibleTimer(elapsedTime);

	// モデル行列更新
	model->UpdateTransform(transform);

	// モデルアニメーション更新処理
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
// 吸い込み用のパラメータ設定
{

	float total_scale_value = (scale.x + scale.y + scale.z);

	// 当たり判定の半径
	//radius = total_scale_value / 3;

	// 吸い込めるようになる最低限のスケール
	//par.enough_total_scale_value = total_scale_value * 0.87f;

	// 消化できるようになる最低限のスケール
	// ※ありえない数値を入れておく
	//par.can_be_digestion_total_scale_value = FLT_MAX;
}