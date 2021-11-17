#include "Enemy.h"

#include "collision.h"
#include "audioManager.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Player.h"

#include "Input/Input.h"
#include "Input/Mouse.h"
#include "easy_math.h"
#include "stageManager.h"


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

	if(attack)
		debugRenderer->DrawCylinder({ float3SUM(position,float3Scaling(GetFront(), radius * 4)) },
			radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));

	switch (enemy_tag)
	{
	case NORMAL_SWORD:
		// 行動範囲
		if(is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(1, 0, 0, 1));
		// 検知範囲
		if(is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(1, 0, 0, 1));
		// 攻撃移行範囲
		if(is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(1, 0, 0, 1));
		break;
	case STRONG_SWORD:
		// 行動範囲
		if (is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(0, 0, 1, 1));
		// 検知範囲
		if (is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(0, 0, 1, 1));
		// 攻撃移行範囲
		if (is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(0, 0, 1, 1));
		break;
	case NORMAL_SPEAR:
		// 行動範囲
		if (is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(0, 1, 0, 1));
		// 検知範囲
		if (is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(0, 1, 0, 1));
		// 攻撃移行範囲
		if (is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(0, 1, 0, 1));
		break;
	case STRONG_SPEAR:
		// 行動範囲
		if (is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(1, 1, 0, 1));
		// 検知範囲
		if (is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(1, 1, 0, 1));
		// 攻撃移行範囲
		if (is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(1, 1, 0, 1));
		break;
	}
}

// デバッグ用GUI描画
void Enemy::DrawDebugGUI(int i)
{
	ImGui::Text("Enemy%d", i);
	ImGui::Text("distance : %.1f", distance);
	ImGui::Text("is_tracking_ : %d", is_tracking_);
	ImGui::Text("is_group_behavior_ : %d", is_group_behavior_);
	ImGui::Text("timer : %d", timer);
	ImGui::Text("is_action_range_ : %d", is_action_range_);
}

void Enemy::UpdateVelocity(float elapsedTime, int kind, const DirectX::XMFLOAT3& playerPos)
{

	float elapsedFrame = 60.0f * elapsedTime;

#ifdef _DEBUG
	//UpdateVerticalVelocity(elapsedFrame, kind);
	//
	//UpdateVerticalMove(elapsedTime, kind);
#else
	UpdateVerticalVelocity(elapsedFrame, kind);

	UpdateVerticalMove(elapsedTime, kind);
#endif


	// 水平移動
	//UpdateHorizontalMove(elapsedTime, playerPos);

	//if (!wall_hit) UpdateMove(elapsedTime, playerPos);


}
//
//void Enemy::UpdateHorizontalMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
//{
//	float Speed;
//	// 移動
//	if(model->GetIndex() != ANIMINDEX::RUN) Speed = this->moveSpeed * elapsedTime;
//	else Speed = (this->moveSpeed * elapsedTime) / 2;
//
//	// 水平速力
//	float veclocityLengthXZ = sqrtf(direction.x * Speed * direction.x * Speed + direction.z * Speed * direction.z * Speed);
//	if (veclocityLengthXZ > 0.0f)
//	{
//
//		float mx = direction.x * Speed;
//		float mz = direction.z * Speed;
//
//		DirectX::XMFLOAT3 start = { position.x,     position.y + stepOffset,    position.z };
//		DirectX::XMFLOAT3 end = { position.x + mx,  position.y + stepOffset,    position.z + mz };
//
//		DirectX::XMFLOAT3 start2 = { position.x,     position.y,    position.z };
//		DirectX::XMFLOAT3 end2 = { position.x + mx,  position.y,    position.z + mz };
//
//		HitResult hit, hit2;
//
//		// レイキャスト2個目完成！
//		//壁があれば
//		if (StageManager::Instance().RayCast(start, end, hit))
//		{
//			// 壁までのベクトル
//			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
//			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//
//			// 壁の法線
//			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
//			Normal = DirectX::XMVector3Normalize(Normal);
//			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);
//
//			DirectX::XMFLOAT3 wall_vec;
//			DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(Vec, DirectX::XMVectorMultiply(Dot, Normal)));
//
//
//
//			start = { position.x,position.y + stepOffset,position.z };
//			end = { position.x + wall_vec.x , position.y + stepOffset, position.z + wall_vec.z };
//
//			// 壁ずり後の位置が壁に
//			// めり込んでいれば
//			if (StageManager::Instance().RayCast(start, end, hit))
//			{
//				// 更新しない
//				position.x += -wall_vec.x;
//				position.z += -wall_vec.z;
//			}
//
//			// めり込んでいなければ
//			else
//			{
//				// 位置の更新
//				/*position.x += wall_vec.x;
//				position.z += wall_vec.z;*/
//				//UpdateMove(elapsedTime, playerPos);
//			}
//
//			if (wall_hit == false)
//			{
//				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Stop();
//				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Play(false);
//			}
//
//			wall_hit = true;
//
//		}
//
//		else if (StageManager::Instance().RayCast(start2, end2, hit2))
//		{
//			// 壁までのベクトル
//			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start2);
//			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end2);
//			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//
//			// 壁の法線
//			DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit2.normal);
//			Normal = DirectX::XMVector3Normalize(Normal);
//			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);
//
//			DirectX::XMFLOAT3 wall_vec;
//			DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(Vec, DirectX::XMVectorMultiply(Dot, Normal)));
//
//
//
//			start = { position.x,position.y,position.z };
//			end = { position.x + wall_vec.x , position.y, position.z + wall_vec.z };
//
//			// 壁ずり後の位置が壁に
//			// めり込んでいれば
//			if (StageManager::Instance().RayCast(start, end, hit))
//			{
//				// 更新しない
//				position.x += -wall_vec.x;
//				position.z += -wall_vec.z;
//			}
//
//			// めり込んでいなければ
//			else
//			{
//				// 位置の更新
//				/*position.x += wall_vec.x;
//				position.z += wall_vec.z;*/
//				//UpdateMove(elapsedTime, playerPos);
//			}
//
//			if (wall_hit == false)
//			{
//				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Stop();
//				AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Play(false);
//			}
//
//			wall_hit = true;
//		}
//
//		//壁がなければ
//		else
//		{
//			// 通常の位置の更新
//			/*position.x += mx;
//			position.z += mz;*/
//
//			//UpdateMove(elapsedTime, playerPos);
//
//			wall_hit = false;
//		}
//
//
//	}
//
//}

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

// プレイヤーに対しての動作の更新
void Enemy::UpdateMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
{
	if (is_dead_ == false)
	{
		distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&playerPos))));


#ifdef _DEBUG
		AttackMove(elapsedTime, playerPos);
		
		RandomMove(elapsedTime);
		
		HomingMove(elapsedTime, playerPos);
#else
		AttackMove(elapsedTime, playerPos);

		RandomMove(elapsedTime);

		HomingMove(elapsedTime, playerPos);
#endif
	}
}

// ランダムな警邏移動
void Enemy::RandomMove(float elapsedTime)
{
	if(is_tracking_ == false && is_group_behavior_ == false)
	{
		if (model->GetIndex() != ANIMINDEX::WALK) model->PlayAnimation(ANIMINDEX::WALK, false);

		is_walking = true;

		float start_position_distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&start_position))));

		if (start_position_distance < kAction_Range)
		{
			if(start_position_distance < kAction_Range / 4) is_action_range_ = true;
		}
		else
		{
			is_action_range_ = false;
			constexpr float zero_avoidance{ 0.0001f }; // ゼロベクトル回避用

			target = {start_position.x + zero_avoidance, start_position.y, start_position.z + zero_avoidance };
		}

		if (is_action_range_)
		{

			if (timer > 60 * 2)
			{
				is_change_direction_ = true;
				timer = 0;
			}

			if (is_change_direction_)
			{
				random_direction = rand() % 4;
				is_change_direction_ = false;
			}

			constexpr int target_range = 100;

			switch (random_direction)
			{
			case 0: // 前
				// 他のtargetによって既に前を向いて歩いているからわざわざtargetの設定をする必要がない
				//random_target.x = 0;
				//random_target.z = target_range;
				break;
			case 1: // 右
				random_target.x = target_range;
				random_target.z = 0;
				break;
			case 2: // 後
				random_target.x = 0;
				random_target.z = -target_range;
				break;
			case 3: // 左
				random_target.x = -target_range;
				random_target.z = 0;
				break;
			}
			target = { this->position.x + random_target.x, this->position.y, this->position.z + random_target.z };
			timer++;
		}

		// 旋回
		{
			float turnSpeed = this->turnSpeed * elapsedTime;

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
				if ((rot) > 0.0001f/*&& fabsf(rot) < 1.0f*/ || /*(rot) > 0.0001f &&*/ is_action_range_ == false) // 後ろを向いているとホーミングしない 
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

				}
			}
		}

		// 移動
		float walkSpeed = (this->moveSpeed * elapsedTime) / 2;
		position.x += direction.x * walkSpeed;
		position.y += direction.y * walkSpeed;
		position.z += direction.z * walkSpeed;

	}
	else
	{
		is_change_direction_ = true;
		timer = 0;
		is_walking = false;
	}
}

// プレイヤーに対しての追尾動作
void Enemy::HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
{
	if (distance < kSensing_Range) is_detection_ = true;
	else is_detection_ = false;

	if (is_detection_ == false && is_tracking_) // 範囲外で追跡中なら通る
	{
		is_tracking_ = false; // 検知範囲外に逃げられたら追跡をやめる
		model->PlayAnimation(ANIMINDEX::IDLE, true);
	}

	// 旋回
	{
		float turnSpeed = this->turnSpeed * elapsedTime;

		if (is_detection_) target = playerPos;

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
			if (((is_detection_  && (rot) > 0.0001f && fabsf(rot) < 1.0f) || is_group_behavior_)) // 検知範囲外か、後ろを向いているとホーミングしない または団体行動中なら通る
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

				if (is_detection_ && is_tracking_ == false) // 範囲内で追跡中でなければ通る
				{
					is_tracking_ = true; // 前側に居たら追跡を開始する
					is_group_behavior_ = false;
					target = playerPos;
					model->PlayAnimation(ANIMINDEX::RUN, true); // 走るモーション
				}
			}
		}
	}

	if ((is_tracking_ || is_group_behavior_) && is_attacking_ == false)
	{
		float RunSpeed = this->moveSpeed * elapsedTime;
		position.x += direction.x * RunSpeed;
		position.y += direction.y * RunSpeed;
		position.z += direction.z * RunSpeed;
	}

}

// プレイヤーに対しての攻撃
void Enemy::AttackMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
{
	if (distance < kAttack_Range)
	{
		is_attacking_ = true;
		if (model->GetIndex() != ANIMINDEX::ATTACK)
		{
			model->PlayAnimation(ANIMINDEX::ATTACK, false);
			animTimer = 0;
			attack = false;
		}
		animTimer++;
		if (animTimer > 38.28f /*5*/) //約0.6秒 
		{
			attack = true;
		}
		//if (model->GetIndex() == ANIMINDEX::ATTACK)
		//if (!model->IsPlayAnimation()) //このifを通っていない->何故分からない…
		//{
		//	animTimer = 0;
		//	attack = false;
		//}
	}
	else if(is_attacking_ && model->IsPlayAnimation() == false)
	{
		attack = false;
		is_attacking_ = false;
		if (model->GetIndex() != ANIMINDEX::RUN) model->PlayAnimation(ANIMINDEX::RUN, true);
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
	model->PlayAnimation(ANIMINDEX::DETH, false);
	is_dead_ = true;
}

// 着地したときに呼ばれる
void Enemy::OnLanding()
{
	if(/*Only_once &&*/ stage_num == 1) // 一度だけ通る
	{
		start_position.y = position.y;
		//Only_once = false;
	}
}