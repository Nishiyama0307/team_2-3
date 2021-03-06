#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "scene/pause.h"

//#include "Audio/Audio.h"

//#define HIT_VOLUME (1.0f)
//#define SHOT_VOLUME (1.0f)

class Player : public Character
{
public:
	Player();
	~Player() override;

	void Update(float elapsedTime, int stage_num, bool explaining = false);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// デバッグプリミティブ描画
	void DrawDebugPrimitive();

	// デバッグ用GUI描画
	void DrawDebugGUI();

public:	// Get関数
	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform._31, transform._32, transform._33); }
	DirectX::XMFLOAT3 GetRight() { return ConvertToIdentityVec(transform._11, transform._12, transform._13); }
	DirectX::XMFLOAT3 GetVelocity() { return velocity; }
	Model* GetModel() { return model; }

	DirectX::XMFLOAT3 front;
public: // Set関数
	void AddImpact(const DirectX::XMFLOAT3 impact_);
	void ResetTransform()
	{
		position = { 0, 0, -155.0f };
		angle = {};


		UpdateTransform();							// オブジェクト行列を更新
		model->UpdateTransform(transform);			// モデル行列更新
	}

private:

	void Input(float elapsedTime);

	// 移動入力値処理
	void InputMove(float elapsedTime);
	//bool InputMove(float elapsedTime);

	// ジャンプ入力処理
	void InputJump();

	// プレイヤーとエネミーとの衝突処理
	void CollisionPlayerVsEnemies();

	void Turn(float elapsedTime, float vx, float vz, float speed)override;

	void Stage1_Gimmick();

	void OnDead();

protected:
	// 着地したときに呼ばれる
	void OnLanding() override;

private:
	DirectX::XMFLOAT3 GetMoveVec() const;
	DirectX::XMFLOAT3 ConvertToIdentityVec(float x, float y, float z)
	{
		DirectX::XMFLOAT3 identity_vec = {};
		DirectX::XMStoreFloat3(&identity_vec, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z))));
		return identity_vec;
	}

private:
	Model* model = nullptr;

	float	moveSpeed = 25.0f;

	float	turnSpeed = DirectX::XMConvertToRadians(720);

	float			jumpspeed = 20.0f;

	int				jumpCount = 0;

#if _DEBUG
	int				jumpLimit = 100;
#else
	int				jumpLimit = 1;
#endif

	//float	velocity_calc = 0;

public:
	/*std::unique_ptr<AudioSource> Shot;
	std::unique_ptr<AudioSource> Hit;*/

private:
	//アニメーション遷移
	enum AnimationPlayer
	{
		Anim_Idel,
		Anim_walk,
		Anim_Run,
		Anim_deth,
		Anim_Damage,
		Anim_Attack1,
		Anim_Attack2,
		Anim_Attack3,
	};
	int anime_state = 0;
	enum class AnimeState
	{
		State_Idel,
		State_walk,
		State_Run,
		State_deth,
		State_Attack1,
		State_Attack2,
		State_Attack3,
		State_Damage,
	};
	AnimeState state = AnimeState::State_Idel;

	float animeTimer = 0;
	bool colstion_check1 = false;
	bool colstion_check2 = false;
	bool colstion_check3 = false;

	//待機ステートへ
	void Idel_change();
	//待機ステート更新
	void UpdateIdel(float elapsedTime);

	//攻撃分岐
	void attack_Branch();
	//アーマー
	bool armor = false;
	//1攻撃ステートへ
	void Attack1_change();
	//1攻撃ステート更新
	void UpdateAttack1(float elapsedTime);
	//2攻撃ステートへ
	void Attack2_change();
	//2攻撃ステート更新
	void UpdateAttack2(float elapsedTime);
	//3攻撃ステートへ
	void Attack3_change();
	//3攻撃ステート更新
	void UpdateAttack3(float elapsedTime);

	//ダメージステート
	void Damage_change();
	//ダメージ更新
	void UpdateDamage(float elapsedTime);
	float damage = 0;

	//死んだステートへ
	//void Deth_change();
	////死んだステート更新
	//void UpdateDeth(float elapsedTime);


	//歩きステートへ
	void Walk_change();
	//歩きステート更新
	void UpdateWalk(float elapsedTime);

	//走りステートへ
	void Run_change();
	//走りステート更新
	void UpdateRun(float elapsedTime);

	//敵の攻撃を喰らう関数
	void EnemyAttckHit();

	//ゼロベクトルかチェック
	bool zeroVec = false;
	//シフトキー離した時のチェック
	bool isbuttn = false;

public:
	//shiftダッシュのスタミナゲージ
	float stamina_limit{ 512 };
	float stamina = stamina_limit;
	float health_limit{ 20 };

	//int kstamina_recovery{ 70 };
	int kstamina_recovery{ 140 };
	int khealth_recovery{ 2 };
	int kself_harm{ 3 };
	int attack1_limit{ 1 }; // 攻撃1 = 1,攻撃2 = 無限, 攻撃3 = 3
	int attack3_limit{ 3 }; // 攻撃1 = 1,攻撃2 = 無限, 攻撃3 = 3
	int attack_count{ 0 };

	int regen_timer{ 0 }; 
	int regen_timer_limit{ 180 }; // 5 * 60

	//マグマで死亡したか
	bool magumaDeath = false;

	//private:
		//std::unique_ptr<Pause> pause = nullptr;
};