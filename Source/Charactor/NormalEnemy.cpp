#include "NormalEnemy.h"
#include "EnemyManager.h"

NormalEnemy::NormalEnemy()
{
	model = new Model("Data/Model/kai_plus.mdl");

	enemy_tag = NORMAL;

	scale.x = scale.y = scale.z = 1.0f;

	// 幅、高さ設定
	height = 2.0f;

	
}

NormalEnemy::~NormalEnemy()
{
	delete model;
	model = NULL;
}

void NormalEnemy::Update(float elapsedTime)
{
	// 速力処理更新
	UpdateVelocity(elapsedTime, KIND::NORMAL_ENEMY);

	// オブジェクト行列を更新
	UpdateTransform();

	// モデル行列更新
	model->UpdateTransform(transform);

}

void NormalEnemy::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	shader->Draw(dc, model);
}

DirectX::XMFLOAT3 NormalEnemy::inhaled()
{
	constexpr float STAGE_1_SCALING_VALUE = 0.1f;

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


void NormalEnemy::SetInhaleParameter()
// 吸い込み用のパラメータ設定
{

	float total_scale_value = (scale.x + scale.y + scale.z);

	// 当たり判定の半径
	radius = total_scale_value / 3;

	// 吸い込めるようになる最低限のスケール
	par.enough_total_scale_value = total_scale_value * 0.87f;

	// 消化できるようになる最低限のスケール
	// ※ありえない数値を入れておく
	par.can_be_digestion_total_scale_value = FLT_MAX;
}