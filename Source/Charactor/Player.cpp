
#include "Player.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Camera.h"
#include "common.h"
#include "EnemyManager.h"
#include "collision.h"
#include "easy_math.h"
#include "Input/Mouse.h"
#include "Scene/title.h"
#include "Scene/scene.h"
extern int attck_select_state;
extern bool f1;
// コンストラクタ
Player::Player()
{
	model = new Model("Data/Model/player/player.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	radius = 5.0f;

	position.y = scale.y + scale.y * 1.7f;

	height = 10.0f;

	//アニメーション再生
	Idel_change();

	colstion_check1 = false;
	colstion_check2 = false;
	colstion_check3 = false;

	//音
	/*Audio& audio1 = Audio::Instance();
	Shot = audio1.LoadAudioSource("Data/Audio/Shot.wav");

	Audio& audio2 = Audio::Instance();
	Hit = audio2.LoadAudioSource("Data/Audio/Hit.wav");*/


	UpdateTransform();							// オブジェクト行列を更新
	model->UpdateTransform(transform);			// モデル行列更新

}

// デストラクタ
Player::~Player()
{
	safe_delete(model);
}


DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// 入力情報を取得
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// カメラ方向とスティックの入力値によって進行方向を計算
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// 移動ベクトルはXZ平面に水平なベクトルになるようにする

	// カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX		= cameraRight.x;
	float cameraRightZ		= cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// 単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX		= cameraFront.x;
	float cameraFrontZ		= cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// スティックの水平入力値をカメラ右方向に反映し、
	// スティックの垂直入力値をカメラ前方向に反映し、
	// 進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = cameraRightX * ax + cameraFrontX * ay;
	vec.z = cameraRightZ * ax + cameraFrontZ * ay;

	// Y軸方向には移動しない
	vec.y = 0.0f;

	return vec;
}

// 更新処理
void Player::Update(float elapsedTime, int stage_num, bool explaining)
{
	//死亡した時アニメーションが終わるまではまだ動ける用にする為のanimdeth
	if (is_dead_)
	{
		if (!model->IsPlayAnimation())
		{
			animdeth = true;
		}
	}
	//
	if (animdeth == false)
	{
		if (explaining == false)
		{
			//Input(elapsedTime);							// 入力処理
			//UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理
		}
		// 速力をなしにする (速力が摩擦以下)
		else
		{

			velocity.x = 0.0f;
			velocity.z = 0.0f;

		}

		if (position.y < -13) jumpspeed = 40.0f;
		else jumpspeed = 20.0f;

		if (stage_num == 0) Stage1_Gimmick();

		front = GetFront();

		EnemyAttckHit();

		//アニメーション再生
		Mouse& mouse = Input::Instance().GetMouse();
		const MouseButton& anyButton = Mouse::BTN_LEFT;

		switch (state)
		{
			//待機
		case AnimeState::State_Idel:
			UpdateIdel(elapsedTime);
			break;

			//攻撃1
		case AnimeState::State_Attack1:
			UpdateAttack1(elapsedTime);
			break;

			//攻撃2
		case AnimeState::State_Attack2:
			UpdateAttack2(elapsedTime);
			break;

			//攻撃3
		case AnimeState::State_Attack3:
			UpdateAttack3(elapsedTime);
			break;

			//ダメージ
		case AnimeState::State_Damage:
			UpdateDamage(elapsedTime);
			break;

			//死亡
		//case AnimeState::State_deth:
		//	UpdateDeth(elapsedTime);
		//	break;

			//歩き
		case AnimeState::State_walk:
			UpdateWalk(elapsedTime);
			break;

			//走り
		case AnimeState::State_Run:
			UpdateRun(elapsedTime);
			break;
		}

		// モデルアニメーション更新処理
		model->UpdateAnimation(elapsedTime);

		// 無敵時間更新
		UpdateInvicibleTimer(elapsedTime);

		// プレイヤーとエネミーとの衝突処理
		CollisionPlayerVsEnemies();

		UpdateTransform();							// オブジェクト行列を更新
		model->UpdateTransform(transform);			// モデル行列更新
	}
}

void Player::Input(float elapsedTime)
{
	// 移動入力処理
	InputMove(elapsedTime);

	// ジャンプ入力処理
	InputJump();

}


void Player::InputMove(float elapsedTime)
{
	// 進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

#if _DEBUG

	// こまちゃんに調整で投げる時用(imguiでいじれるように更新を止める)
#if 0
	//// 移動処理
	//switch (state)
	//{
	//	////走ってるとき
	//case AnimeState::State_Run:
	//	moveSpeed = 200;
	//	break;
	//	//それ以外
	//default:
	//	moveSpeed = 100;
	//	break;
	//}
#else

	// 移動処理
	switch (state)
	{
		////走ってるとき
	case AnimeState::State_Run:
		moveSpeed = 200;
		break;
		//それ以外
	default:
		moveSpeed = 100;
		break;
	}
#endif


#else
	// 移動処理
	switch (state)
	{
		////走ってるとき
		case AnimeState::State_Run:
			if (stamina == 0)
				moveSpeed = 10;
			else
				moveSpeed = 50;

			break;
		//それ以外
		default:
			moveSpeed = 25;
			break;
			
	}

#endif

	Move(moveVec.x, moveVec.z, moveSpeed);

	//Turn(elapsedTime, 0, 0, turnSpeed);
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	//走りに移行する為チェック
	float moveVecLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&moveVec)));
	if (moveVecLength > 0.0f)
		zeroVec = true;
	else
		zeroVec = false;
}

// ジャンプ入力処理
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
	{
		if (jumpCount < jumpLimit)
		{
			Jump(jumpspeed);
			jumpCount++;
		}
	}
}

// 着地したときに呼ばれる
void Player::OnLanding()
{
	jumpCount = 0;
}

void Player::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// カメラ方向とスティックの入力値によって進行方向を計算
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// 移動ベクトルはXZ平面に水平なベクトルになるようにする

	// カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// 単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = frontX * cameraFrontX + frontZ * cameraFrontZ; // 内積
	float rot = 1.0 - dot; // 回転量

	// 内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	if (rot > speed) rot = speed;


	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	// A.x * B.y - A.z * B.x
	float cross = frontX * cameraFrontZ - frontZ * cameraFrontX;

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える


	// 左右判定を行うことによって回転方向を選択する
	if (cross < 0.0f)	// 時計回り
	{
		angle.y += rot;
	}
	else				// 反時計周り
	{
		angle.y += -rot;
	}

}

void Player::Stage1_Gimmick()
{

	if ((position.y > 22 || position.y <= -15) && isGround) ApplyDamage(1, 2.0f);

	//要らなくなるかも
	//if (position.y >= 35) position.y = 35;
	//if (position.y >= 30) jumpCount = 100;
}

void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// 描画処理
	shader->Draw(dc, model);
}

// プレイヤーとエネミーとの衝突処理
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// 全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		// 球と球の衝突処理
		//DirectX::XMFLOAT3 outPosition;
		//if (Collision::IntersectSqhereVsSqhere(
		//	position,
		//	radius,
		//	enemy->GetPosition(),
		//	enemy->GetRadius(),
		//	outPosition
		//))
		//{
		//	//押し出し後の位置設定
		//	enemy->SetPosition(outPosition);
		//}

		// 円柱と円柱の衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition
		))
		{
			//DirectX::XMFLOAT3 up = { 0.0001f,1,0 };

			// 真上から踏んだか調べる
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);

			/*float vy  = position.y - enemy->GetPosition().y;
			vy = sqrtf(vy * vy);
			if (vy > 0.8f)*/

			if (normal.y > 0.8f) // 0.8は経験則
			{
				Jump(jumpspeed * 0.75f); // ジャンプ量
				// ダメージを与える
				//enemy->ApplyDamage(1, 0.5f);
			}
			else
			{
				//押し出し後の位置設定
				enemy->SetPosition(outPosition);
			}
		}
	}
}

void Player::DrawDebugPrimitive()
{
	// デバッグプリミティブ描画
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	
	// 衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(hit_position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));


	debugRenderer->DrawSphere({ position.x,position.y + stepOffset,position.z }, 0.1f, DirectX::XMFLOAT4(0, 0, 0, 1));

	//攻撃１
	if(colstion_check1)
		debugRenderer->DrawCylinder({ float3SUM(position,float3Scaling(GetFront(), radius * 2)) },
		radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//攻撃２
	if (colstion_check2)
		debugRenderer->DrawCylinder({ position.x,position.y,position.z},
			12.5, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//攻撃３
	if (colstion_check3)
		debugRenderer->DrawCylinder({ float3SUM(position,float3Scaling(GetFront(), radius * 2)) },
			radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

}


void Player::DrawDebugGUI()
{
	// デバッグ用GUI描画
	ImGui::Text("param.hp : %d", par.health);

	ImGui::SliderFloat("moveSpeed", &moveSpeed, 0.0f, +100.0f);

	ImGui::SliderFloat("position.y", &position.y, 0.0f, +500.0f);

}


void Player::AddImpact(const DirectX::XMFLOAT3 impact_)
{

	this->velocity.x += impact_.x;
	this->velocity.y += impact_.y;
	this->velocity.z += impact_.z;
}


////アニメーションのステート関係
//攻撃分岐
void Player::attack_Branch()
{
	//攻撃選択によってアニメーションが変わる
	Mouse& mouse = Input::Instance().GetMouse();
	const MouseButton& anyButton = Mouse::BTN_LEFT;
	if (mouse.GetButtonDown() & anyButton)
	{
		armor = true;
		switch (attck_select_state)
		{
		case 0:
			Attack1_change();
			break;
		case 1:
			Attack2_change();
			break;
		case 2:
			Attack3_change();
			break;
		}
	}
}

//待機ステートへ
void Player::Idel_change()
{
	state = AnimeState::State_Idel;
	model->PlayAnimation(Anim_Idel, true);
}

//待機ステート更新
void Player::UpdateIdel(float elapsedTime)
{
	stamina += 20 * elapsedTime;
	if (stamina > 512)
		stamina = 512;

	//ポーズ解除時、攻撃とポーズ解除の左クリックが同時に処理されるので１フレームだけ動かさない
	if (f1)
	{
		Input(elapsedTime);							// 入力処理
		UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理

		GamePad& pad = Input::Instance().GetGamePad();
		const GamePadButton button = GamePad::BTN_SHIFT;
		if (zeroVec)
		{
			Walk_change();
		}

		attack_Branch();
	}
	f1 = true;
}

//攻撃ステートへ  1
void Player::Attack1_change()
{
	animeTimer = 0;
	state = AnimeState::State_Attack1;
	model->PlayAnimation(Anim_Attack1, false);
}
//攻撃ステート更新  1
void Player::UpdateAttack1(float elapsedTime)
{
	animeTimer++;
	//colstion_check1 = true;
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 attackPosition;
	attackPosition = { float3SUM(position,float3Scaling(GetFront(), radius * 2)) };
	if (animeTimer > 12.0f)colstion_check1 = true;
	if (animeTimer > 33.0f)colstion_check1 = false;

	if (colstion_check1)
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// 円柱と円柱の衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			attackPosition, radius, height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition))
		{
			enemy->ApplyDamage(10, 5);
		}

	}

	if (!model->IsPlayAnimation())
	{
		armor = false;
		colstion_check1 = false;
		Idel_change();
	}
}

//攻撃ステートへ  2
void Player::Attack2_change()
{
	animeTimer = 0;
	state = AnimeState::State_Attack2;
	model->PlayAnimation(Anim_Attack2, false);
}
//攻撃ステート更新  2
void Player::UpdateAttack2(float elapsedTime)
{
	animeTimer++;
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 attackPosition;
	attackPosition.x = position.x;
	attackPosition.y = position.y;
	attackPosition.z = position.z;

	//attackPosition = { float3SUM(position,float3Scaling(GetFront(), radius * 2)) };

	if (animeTimer > 24.0f)colstion_check2 = true;
	if (animeTimer > 87.0f)colstion_check2 = false;

	if(colstion_check2)
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// 円柱と円柱の衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			attackPosition, 12.5, height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition))
		{
			enemy->ApplyDamage(10, 5);
		}
	}
	if (!model->IsPlayAnimation())
	{
		colstion_check2 = false;
		armor = false;
		Idel_change();
	}
}

//攻撃ステートへ  3
void Player::Attack3_change()
{
	animeTimer = 0;
	state = AnimeState::State_Attack3;
	model->PlayAnimation(Anim_Attack3, false);
}
//攻撃ステート更新  3
void Player::UpdateAttack3(float elapsedTime)
{
	animeTimer++;
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 attackPosition;
	attackPosition = { float3SUM(position,float3Scaling(GetFront(), radius * 2)) };

	if(animeTimer > 135.98f)colstion_check3 = true;
	if(animeTimer > 148.98f)colstion_check3 = false;


	if(colstion_check3)
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// 円柱と円柱の衝突処理
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			attackPosition, radius, height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition))
		{
			enemy->ApplyDamage(10, 5);
		}
	}
	if (!model->IsPlayAnimation())
	{
		colstion_check3 = false;
		armor = false;
		Idel_change();
	}
}

//ダメージステートへ
void Player::Damage_change()
{
	state = AnimeState::State_Damage;
	if(model->GetIndex() != Anim_Damage)
	model->PlayAnimation(Anim_Damage, false);
	ApplyDamage(damage, 1.0f);
}
//ダメージ更新
void Player::UpdateDamage(float elapsedTime)
{
	if (!model->IsPlayAnimation())
	{
		Idel_change();
	}
	Input(elapsedTime);							// 入力処理
	UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理
	
	if (f1)
	{
		attack_Branch();
	}
	f1 = true;
}

//死亡
void Player::OnDead()
{
	model->PlayAnimation(Anim_deth, false);
	is_dead_ = true;
}


//死亡ステートへ
//void Player::Deth_change()
//{
//	state = AnimeState::State_deth;
//	model->PlayAnimation(Anim_deth, false);
//}
////死亡ステート更新
//void Player::UpdateDeth(float elapsedTime)
//{
//	if (model->IsPlayAnimation())
//	{
//		
//	}
//}

// 歩きステートへ
void Player::Walk_change()
{
	state = AnimeState::State_walk;
	model->PlayAnimation(Anim_walk, true);
}
//歩きステート更新
void Player::UpdateWalk(float elapsedTime)
{
	Input(elapsedTime);							// 入力処理
	UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理

	stamina += 20 * elapsedTime;
	if (stamina > 512)
		stamina = 512;

	GamePad& pad = Input::Instance().GetGamePad();
	const GamePadButton button = GamePad::BTN_SHIFT;
	//入力してない時は待機
	if (!zeroVec)
	{
		Idel_change();
	}
	//shiftキーでダッシュ
	if (pad.GetButtonDown() & button)
	{
		isbuttn = false;	//ボタンを押した状態を引き継ぐ為
		Run_change();	//ステートのチェンジ
	}
	if (f1)
	{
		attack_Branch();
	}
	f1 = true;
}

//走りステートへ
void Player::Run_change()
{
	state = AnimeState::State_Run;
	model->PlayAnimation(Anim_Run, true);
}
//走りステート更新
void Player::UpdateRun(float elapsedTime)
{
	Input(elapsedTime);							// 入力処理
	UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理

	//スタミナ
	stamina -= 40 * elapsedTime;
	if (stamina < 0)
		stamina = 0;

	GamePad& pad = Input::Instance().GetGamePad();
	const GamePadButton button = GamePad::BTN_SHIFT;
	//ボタンを離した時
	if (pad.GetButtonUp() & button)
	{
		isbuttn = true;
	}
	if (isbuttn)
	{
		isbuttn = false;
		Walk_change();
	}
	//入力してない時は待機
	if (!zeroVec)
	{
		isbuttn = false;
		Idel_change();
	}
	if (f1)
	{
		attack_Branch();
	}
	f1 = true;

}

//敵の攻撃を食らう
void Player::EnemyAttckHit()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// 全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		DirectX::XMFLOAT3 attackPosition;
		attackPosition = { float3SUM(enemy->GetPosition(),float3Scaling(enemy->GetFront(), enemy->GetRadius() * 4)) };

		if (enemy->attack)
		{
			// 円柱と円柱の衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision3D::IntersectCylinderVsCylinder(
				position,
				radius,
				height,
				attackPosition,
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition
			))
			{
#if 0
				//敵毎のダメージ分岐
				switch (enemy->enemy_tag)
				{
				case NORMAL_SWORD:
					damage = enemy->par.power;
					break;
				case STRONG_SWORD:
					damage = enemy->par.power;
					break;
				case NORMAL_SPEAR:
					damage = enemy->par.power;
					break;
				case STRONG_SPEAR:
					damage = enemy->par.power;
					break;
				}
#endif
				damage = enemy->par.power;

				//死亡していない時
				if (!is_dead_)
				{
					//攻撃モーションしていない(アーマーがついていない)時はアニメーションする
					if (!armor)
						Damage_change();
					//攻撃モーション中(アーマーがついている)時はアニメーションせずそのままダメージ
					if (armor)
						ApplyDamage(enemy->par.power, 1.0f);
				}
			}
		}
	}
}