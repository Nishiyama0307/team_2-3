#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
class Player;


enum ENEMYTAG
{
	NORMAL_SWORD,
	STRONG_SWORD,
	NORMAL_SPEAR,
	STRONG_SPEAR,
};


class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override {}

	//virtual void Update(float elapsedTime) = 0;
	virtual void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos = {0,0,0}) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	void UpdateVelocity(float elapsedTime, int kind)override;

	// inhaled(形容詞で吸い込まれた)
	virtual DirectX::XMFLOAT3 inhaled() = 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI(int i);

	// プレイヤーに対しての追尾動作
	void HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// 行列更新処理
	void UpdateTransform();

	// 破棄
	void Destroy();

	// 死亡
	void OnDead();

	//// 追跡が外れた
	//void OnNonTracking();

public: // Get関数
public: // Set関数
	void SetPlayerAddress(Player* player_) { player = player_; }
	virtual void SetInhaleParameter() = 0;

public: // length
	void SetLengthSq(float sq) { lengthSq = sq; }
	float GetLengthSq() const { return lengthSq; }

public:
	int enemy_tag;

	float lengthSq;

	bool Is_inhaling = false; // Is_inhaling(吸入中) = 吸い込まれてるかどうか
	Player* player = nullptr;


	DirectX::XMFLOAT3	target = { 0, 0, 0 };	
	DirectX::XMFLOAT3	direction = { 0,0,-1 };							// 見ている方向
	float				moveSpeed = 10.0f;										// 移動速度
	float				turnSpeed = DirectX::XMConvertToRadians(180);	// 回転速度
	float				distance = 100;											// playerとenemyの距離
	float				Sensing_range = 50;										// 検知範囲

	bool is_tracking_ = false;													// 追跡してるかどうか
	bool is_detection_ = false;													// 検知範囲に居るかどうか
	bool is_group_behavior_ = false;
	
};