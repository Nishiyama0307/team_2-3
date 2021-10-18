
#include "Player.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Camera.h"
#include "common.h"
#include "EnemyManager.h"
#include "collision.h"
#include "easy_math.h"



// コンストラクタ
Player::Player()
{
	model = new Model("Data/Model/player/test_player.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	radius = 5.0f;

	hit_correction = 5.0f;

	inhale = new Inhale(this);

	position.y = scale.y + scale.y * 1.7f;

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
		Input(elapsedTime);							// 入力処理
		UpdateVelocity(elapsedTime, KIND::PLAYER);	// 速力更新処理
	}
	// 速力をなしにする (速力が摩擦以下)
	else
	{

		velocity.x = 0.0f;
		velocity.z = 0.0f;

	}

	inhale->Update(elapsedTime);				// 掃除機機能の更新

	SetHitPosition(position);

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
				enemy->ApplyDamage(1, 0.5f);
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
	
	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawSphere(hit_position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));


	debugRenderer->DrawSphere({ hit_position.x,hit_position.y + stepOffset,hit_position.z }, 0.1f, DirectX::XMFLOAT4(0, 0, 0, 1));

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
