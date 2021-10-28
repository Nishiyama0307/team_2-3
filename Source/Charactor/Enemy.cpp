#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Player.h"

// デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	
	// 当たり判定の大きさ
	radius = 1.0f;

	// 幅、高さ設定
	height = 6.0f;
	
	// 衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(hit_position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// デバッグ用GUI描画
void Enemy::DrawDebugGUI(int i)
{
	ImGui::Text("Enemy%d", i);
	ImGui::Text("distance : %.1f", distance);
	ImGui::Text("is_tracking_ : %d", is_tracking_);
	ImGui::Text("is_group_behavior_ : %d", is_group_behavior_);
}

void Enemy::UpdateVelocity(float elapsedTime, int kind)
{

	float elapsedFrame = 60.0f * elapsedTime;

	UpdateVerticalVelocity(elapsedFrame, kind);

	UpdateVerticalMove(elapsedTime, kind);


	if (Is_inhaling == false) return;


}

// 行列更新処理
void Enemy::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	// 前ベクトルを算出
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	// 仮の上ベクトルを算出
	Up = DirectX::XMVectorSet(0.0001f, 1, 0, 0);// ちょっとずらす({0, 1, 0}はよく使うため)
	Up = DirectX::XMVector3Normalize(Up);

	// 右ベクトルを算出(この場合長さが1になっている)
	Right = DirectX::XMVector3Cross(Up, Front);

	// 上ベクトルを算出
	Up = DirectX::XMVector3Cross(Front, Right);

	// 計算結果を取り出し
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	// 算出した軸ベクトルから行列を作成
	transform._11 = right.x * scale.x;
	transform._12 = right.y * scale.x;
	transform._13 = right.z * scale.x;
	transform._14 = 0.0f;
	transform._21 = up.x * scale.y;
	transform._22 = up.y * scale.y;
	transform._23 = up.z * scale.y;
	transform._24 = 0.0f;
	transform._31 = front.x * scale.z;
	transform._32 = front.y * scale.z;
	transform._33 = front.z * scale.z;
	transform._34 = 0.0f;
	transform._41 = position.x;
	transform._42 = position.y;
	transform._43 = position.z;
	transform._44 = 1.0f;

	// 発射方向
	this->direction = front;
}


// プレイヤーに対しての追尾動作
void Enemy::HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
{
	distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&playerPos))));

	if (distance < Sensing_range) is_detection_ = true;
	else is_detection_ = false;

	if (is_tracking_) is_group_behavior_ = false;

	

	if (is_detection_ == false && is_tracking_) // 範囲外で追跡中なら通る
	{
		is_tracking_ = false; // 検知範囲外に逃げられたら追跡をやめる
		model->PlayAnimation(0, true);
	}

	// 旋回
	{
		float turnSpeed = this->turnSpeed * elapsedTime;

		if(is_detection_) target = playerPos;

		// ターゲットまでのベクトルを算出
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

		// ゼロベクトルでないなら回転処理
		DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
		float lengthSq;
		DirectX::XMStoreFloat(&lengthSq, LengthSq);


		if (lengthSq > 0.00001f)
		{
			// ターゲットまでのベクトルを単位ベクトル化
			Vec = DirectX::XMVector3Normalize(Vec);


			// 向いている方向ベクトルを算出
			DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

			// 前方方向ベクトルとターゲットまでのベクトルの内積（角度）を算出
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

			float dot;
			DirectX::XMStoreFloat(&dot, Dot);

			// ２つの単位ベクトルの角度が小さいほど
			// 1.0に近づくという性質を利用して回転速度を調整する
			float rot = 1.0f - dot; // 0~2までの値になっている
			if (rot > turnSpeed && rot < 1.0f) // 後ろを向いているとホーミングしない
			{
				rot = turnSpeed;
			}

			// 回転角度があるなら回転処理をする
			if (((is_detection_ || is_group_behavior_) && (rot) > 0.0001f && fabsf(rot) < 1.0f) ) // 検知範囲外か、後ろを向いているとホーミングしない または団体行動中なら通る
			{
				// 回転軸を算出
				DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
				// 回転軸と回転量から回転行列を算出
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
				// 現在の行列を回転させる
				DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
				Transform = DirectX::XMMatrixMultiply(Transform, Rotation);
				// 回転後の前方方向を取り出し、単位ベクトル化する
				Direction = DirectX::XMVector3Normalize(Transform.r[2]);
				DirectX::XMStoreFloat3(&direction, Direction);

				// 移動
				if (is_tracking_ || is_group_behavior_) // 追跡中か団体行動時のみ移動
				{
					float moveSpeed = this->moveSpeed * elapsedTime;
					position.x += direction.x * moveSpeed;
					position.y += direction.y * moveSpeed;
					position.z += direction.z * moveSpeed;
				}

				if (is_detection_ && is_tracking_ == false) // 範囲内で追跡中でなければ通る
				{
					is_tracking_ = true; // 前側に居たら追跡を開始する
					is_group_behavior_ = false;
					target = playerPos;
					model->PlayAnimation(2, true); // 走るモーション
				}
			}
		}
	}

}

// 破棄
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}


// 死亡
void Enemy::OnDead()
{
	model->PlayAnimation(1, true);
}

//// 追跡が外れた
//void Enemy::OnNonTracking()
//{
//	is_tracking_ = false; // 検知範囲外に逃げられたら追跡をやめる
//}