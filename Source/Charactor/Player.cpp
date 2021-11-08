
#include "Player.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Camera.h"
#include "common.h"
#include "EnemyManager.h"
#include "collision.h"
#include "easy_math.h"
#include "Input/Mouse.h"

extern int attck_select_state;
extern bool f1;
// コンストラクタ
Player::Player()
{
	model = new Model("Data/Model/player/player.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	radius = 5.0f;

	inhale = new Inhale(this);

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
	safe_delete(inhale);
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
void Player::Update(float elapsedTime, bool explaining)
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

	if (position.y < 0) jumpspeed = 40.0f;
	else jumpspeed = 20.0f;

	inhale->Update(elapsedTime);				// 掃除機機能の更新

	front = GetFront();

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

		//死亡
	case AnimeState::State_deth:
		UpdateDeth(elapsedTime);
		break;

		//走り
	case AnimeState::State_Run:
		UpdateRun(elapsedTime);
		break;
	}

	// モデルアニメーション更新処理
	model->UpdateAnimation(elapsedTime);


	// プレイヤーとエネミーとの衝突処理
	CollisionPlayerVsEnemies();

	UpdateTransform();							// オブジェクト行列を更新
	model->UpdateTransform(transform);			// モデル行列更新
}

void Player::Input(float elapsedTime)
{
	// 移動入力処理
	InputMove(elapsedTime);

	// ジャンプ入力処理
	InputJump();

	// 吸い込みの入力処理
	//InputInhale();
}


void Player::InputMove(float elapsedTime)
{
	// 進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// 移動処理
	Move(moveVec.x, moveVec.z, moveSpeed);

	//Turn(elapsedTime, 0, 0, turnSpeed);
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
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


void Player::InputInhale()
{
	inhale->InputInhale();

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

	inhale->DebugRender();
}


void Player::DrawDebugGUI()
{
	// デバッグ用GUI描画
}


void Player::AddImpact(const DirectX::XMFLOAT3 impact_)
{

	this->velocity.x += impact_.x;
	this->velocity.y += impact_.y;
	this->velocity.z += impact_.z;
}


////アニメーションのステート関係
//待機ステートへ
void Player::Idel_change()
{
	state = AnimeState::State_Idel;
	model->PlayAnimation(Anim_Idel, true);
}

//待機ステート更新
void Player::UpdateIdel(float elapsedTime)
{
	if (f1)
	{
		Input(elapsedTime);							// 入力処理
		UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理

		//攻撃選択によってアニメーションが変わる
		Mouse& mouse = Input::Instance().GetMouse();
		const MouseButton& anyButton = Mouse::BTN_LEFT;
		if (mouse.GetButtonDown() & anyButton)
		{
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
	//attackPosition.x = position.x;
	//attackPosition.y = position.y;
	//attackPosition.z = position.z + radius *2;

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
		Idel_change();
	}
}

//デスステートへ
void Player::Deth_change()
{
	state = AnimeState::State_deth;
}
//デスステート更新
void Player::UpdateDeth(float elapsedTime)
{
	model->PlayAnimation(Anim_deth, false);
}

//走りステートへ
void Player::Run_change()
{
	state = AnimeState::State_Run;
}
//走りステート更新
void Player::UpdateRun(float elapsedTime)
{
	Input(elapsedTime);							// 入力処理
	UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理

	model->PlayAnimation(Anim_Run, true);
}