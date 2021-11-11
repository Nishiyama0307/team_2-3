#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
class Player;


constexpr float				kSensing_Range = 50;									// 検知範囲
constexpr float				kAttack_Range = 10;										// 攻撃範囲
constexpr float				kAction_Range = 20;										// 行動範囲

enum ENEMYTAG
{
	NORMAL_SWORD,
	STRONG_SWORD,
	NORMAL_SPEAR,
	STRONG_SPEAR,
};

enum ANIMINDEX
{
	IDLE,
	WALK,
	RUN,
	ATTACK,
	DETH,
};

class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override {}

	//virtual void Update(float elapsedTime) = 0;
	virtual void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos = {0,0,0}) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	void UpdateVelocity(float elapsedTime, int kind, const DirectX::XMFLOAT3& playerPos);
	//void UpdateHorizontalMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// inhaled(形容詞で吸い込まれた)
	virtual DirectX::XMFLOAT3 inhaled() = 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI(int i);

	// プレイヤーに対しての動作の更新
	void UpdateMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// ランダムな警邏移動
	void RandomMove(float elapsedTime);

	// プレイヤーに対しての追尾動作
	void HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// プレイヤーに対しての攻撃
	void AttackMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// 行列更新処理
	void UpdateTransform();

	// 破棄
	void Destroy();

	// 死亡
	void OnDead();

	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform._31, transform._32, transform._33); }

public: // Get関数
public: // Set関数
	void SetPlayerAddress(Player* player_) { player = player_; }
	virtual void SetInhaleParameter() = 0;
	void SetStageNum(int stage_num) { this->stage_num = stage_num; }

public: // length
	void SetLengthSq(float sq) { lengthSq = sq; }
	float GetLengthSq() const { return lengthSq; }

public:
	ENEMYTAG enemy_tag;

	float lengthSq;

	bool Is_inhaling = false; // Is_inhaling(吸入中) = 吸い込まれてるかどうか
	Player* player = nullptr;


	DirectX::XMFLOAT3	start_position = { 0, 0, 0 };					// 初期位置
	DirectX::XMFLOAT3	target = { 0, 0, 0 };	
	DirectX::XMFLOAT3	direction = { 0,0,-1 };							// 見ている方向
	DirectX::XMFLOAT3	random_target = { 0,0,0 };							// 見ている方向
	float				moveSpeed = 10.0f;										// 移動速度
	float				turnSpeed = DirectX::XMConvertToRadians(180);	// 回転速度
	float				distance = 100;											// playerとenemyの距離

	bool is_tracking_ = false;													// 追跡してるかどうか
	bool is_detection_ = false;													// 検知範囲に居るかどうか
	bool is_group_behavior_ = false;											// 団体行動してるかどうか
	bool is_attacking_ = false;													// 攻撃してるかどうか
	bool is_walking = true;													// 歩いているかどうか
	bool is_change_direction_ = false;											// 歩く方向を変えれるかどうか
	bool is_action_range_ = true;												// 行動範囲にいるかどうか

	int timer{ 0 };
	int random_direction{ 0 };
	int stage_num;

private:
	DirectX::XMFLOAT3 ConvertToIdentityVec(float x, float y, float z)
	{
		DirectX::XMFLOAT3 identity_vec = {};
		DirectX::XMStoreFloat3(&identity_vec, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z))));
		return identity_vec;
	}
	
};