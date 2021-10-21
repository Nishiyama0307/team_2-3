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

	virtual void Update(float elapsedTime) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	void UpdateVelocity(float elapsedTime, int kind)override;

	// inhaled(形容詞で吸い込まれた)
	virtual DirectX::XMFLOAT3 inhaled() = 0;

	// デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();

	// 破棄
	void Destroy();

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

	
};