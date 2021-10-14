#pragma once

#include <DirectXMath.h>



struct Parameter
{
	DirectX::XMFLOAT3 scaling_value = {0,0,0};		// 吸い込んだ物によって＋or－
	float enough_total_scale_value = 0.0f;			// 吸い込める最低ライン
	float can_be_digestion_total_scale_value = 0.0f;// 消化できる最低ライン
};


// キャラクター	
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	// 行列更新処理
	void UpdateTransform();

	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// 半径取得
	float GetRadius() const { return radius; }

	// 高さ取得
	float GetHeight() const { return height; }

	// 地面に接地しているか
	bool IsGround() const { return isGround; }

	// ダメージを与える
	bool ApplyDamage(int damage, float invincibleTime);

	// 衝撃を与える
	void AddImpulse(const DirectX::XMFLOAT3& impulse);
protected:

	// 移動処理
	void Move(float vx, float vz, float speed);

	// 旋回処理
	virtual void Turn(float elapsedTime, float vx, float vz, float speed);

	// ジャンプ処理
	void Jump(float speed);

	// 速力処理更新
	virtual void UpdateVelocity(float elapsedTime, int kind);


protected:
	// 垂直速力の更新
	void UpdateVerticalVelocity(float elapsedFrame, int kind);
	// 垂直移動の更新
	void UpdateVerticalMove(float elapsedTime, int kind = 0);
	// 水平速力の更新
	void UpdateHorizontalVelocity(float elapsedFrame);
	// 水平移動の更新
	void UpdateHorizontalMove(float elapsedTime);

	// 無敵時間更新
	void UpdateInvicibleTimer(float elapsedTime);

	// 着地した時に呼ばれる
	virtual void OnLanding() {}

	// ダメージを受けたときに呼ばれる
	virtual void OnDamaged() {}

	// 死亡したときに呼ばれる
	virtual void OnDead() {}

protected:
	DirectX::XMFLOAT3	position = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	DirectX::XMFLOAT3 foot_pos = { 0,0,0 };
	
	float				radius = 0.5f;

	float				gravity = -1.0f;
	DirectX::XMFLOAT3	velocity = { 0,0,0 };

	bool				isGround = false;

	int					health = 5;

	float				height = 2.0f;

	float				invincibleTimer = 1.0f; // 無敵時間

	float friction = 0.5f; // 摩擦力
	float acceleration = 1.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	float				airControl = 0.3f;

	float stepOffset = 1.0f;

	enum KIND{
		PLAYER,		// プレイヤー
		NORMAL_ENEMY, // ノーマルモブ
		BOMB_ENEMY,	// デメリットモブ
		RARE_ENEMY,	// レアモブ
	};

	bool wall_hit = false;

public:
	Parameter par;
};