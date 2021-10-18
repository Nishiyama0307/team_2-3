#include "Character.h"
#include "collision.h"
#include "stageManager.h"

#include "audioManager.h"


// 行列更新処理
void Character::UpdateTransform()
{
	// スケール行列を作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列を作成
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// 位置行列を作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// ３つの行列を組み合わせ、ワールド行列を作成
	DirectX::XMMATRIX W = S * R * T;
	// 計算したワールド行列を取り出す
	DirectX::XMStoreFloat4x4(&transform, W);
}

// ダメージを与える
bool Character::ApplyDamage(int damage, float invincibleTime)
{
    // ダメージが0の場合は健康状態を変更する必要がない
    if (damage == 0) return false;

    // 死亡している場合は健康状態を変更しない
    if (health == 0) return false;

    // ダメージ処理
    if (invincibleTimer < 0.1f)
    {
        health -= damage;
        invincibleTimer = invincibleTime;
    }

    // 死亡通知
    if (health == 0)
    {
        OnDead();
    }
    else
        // ダメージ通知
    {
        OnDamaged();
    }

    // 健康状態が変更した場合はtrueを返す

    return true;
}

// 衝撃を与える
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    // 速力に力を加える
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

// 移動処理
void Character::Move(float vx, float vz, float speed)
{
    // 移動方向のベクトル設定
    moveVecX = vx;
    moveVecZ = vz;

    // 最大速度の設定
    maxMoveSpeed = speed;
}

// 旋回処理
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// 進行ベクトルがゼロベクトルの場合は処理する必要なし
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.0001f) return;

	// 進行ベクトルを単位ベクトル化
	vx /= length;
	vz /= length;

	// 自身の回転値から前方向を求める
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// 回転角を求めるため、２つの単位ベクトルの内積を計算する
	// A.x * B.x + A.z * B.z
	float dot = frontX * vx + frontZ * vz; // 内積
	float rot = 1.0 - dot; // 補正値

	// 内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	// 小さいほど1.0に近づくという性質を利用して回転速度を調整する
	if (rot < speed) speed = rot;


	// 左右判定を行うために２つの単位ベクトルの外積を計算する
	// A.x * B.y - A.z * B.x
	float cross = frontX * vz - frontZ * vx;

	// 2Dの外積値が正の場合か負の場合によって左右判定が行える


	// 左右判定を行うことによって左右回転を選択する
	if (cross <= 0.0f) // 右に旋回
	{
		angle.y += speed;
	}
	else if (cross > 0.0f) // 左に旋回
	{
		angle.y -= speed;
	}

}

// ジャンプ処理
void Character::Jump(float speed)
{
	// 上方向の力を設定
	velocity.y = speed;
}

// 速力処理更新
void Character::UpdateVelocity(float elapsedTime, int kind)
{
    //　経過フレーム
    float elapsedFrame = 60.0f * elapsedTime;

    // 垂直速力
    UpdateVerticalVelocity(elapsedFrame, kind);

    // 水平速力
    UpdateHorizontalVelocity(elapsedFrame);


    // 垂直移動
    UpdateVerticalMove(elapsedTime);

    // 水平移動
    UpdateHorizontalMove(elapsedTime);
}

// 垂直速力更新処理
void Character::UpdateVerticalVelocity(float elapsedFrame, int kind)
{
    // 重力処理
    velocity.y += gravity * elapsedFrame;
}

// 垂直移動更新処理
void Character::UpdateVerticalMove(float elapsedTime, int kind)
{

    //// 移動処理
    //position.y += velocity.y * elapsedTime;

    //float offset = 0.0f;
    ////if (kind == 0) offset = scale.y * 1.7f;
    ////if (kind > 0) 
    //offset = -scale.y * 0.3f;

    //foot_pos.y = position.y - scale.y - offset;

    //// 地面判定
    //if (foot_pos.y < 0.0f)
    //{
    //    foot_pos.y = 0.0f;
    //    position.y = scale.y + offset;

    //    velocity.y = 0.0f;

    //    isGround = true;
    //}
    //else
    //{
    //    isGround = false;
    //}

    // 垂直方向の移動量
    float my = velocity.y * elapsedTime;

    // 落下中
    if (my < 0.0f)
    {
        // レイの開始位置は足元より少し上
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
        // レイの終点位置は移動後の位置
        DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

        // レイキャストによる地面判定
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // 地面に接地している
            position.y = hit.position.y;

            // 着地した
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            // 空中に浮いている
            position.y += my;
            isGround = false;
        }
    }
    // 上昇中
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
}


void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // XZの力の強さ(速力)を算出
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    // 速力がある時 (摩擦処理をする)
    if (length > 0.0f)
    {

        // 摩擦力
        float friction = this->friction * elapsedFrame;

        // 空中なら摩擦力を減らす
        if (!IsGround()) friction *= 0.7f; // ３割減

        // 摩擦による減速処理 (速力が摩擦以上)
        if (length > friction)
        {
            DirectX::XMVECTOR n_Velo = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity));


            DirectX::XMFLOAT3 normalized_velo;
            DirectX::XMStoreFloat3(&normalized_velo, n_Velo);

            float vx = normalized_velo.x;
            float vz = normalized_velo.z;

            velocity.x = velocity.x - vx * friction;
            velocity.z = velocity.z - vz * friction;

        }

        // 速力をなしにする (速力が摩擦以下)
        else
        {

            velocity.x = 0.0f;
            velocity.z = 0.0f;

        }

    }


    // 速力が最大速度以内のとき　(加速処理をする)
    if (length <= maxMoveSpeed)
    {
        // 移動ベクトルの算出
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        //移動ベクトルがある時 (加速処理をする)
        if (moveVecLength > 0.0f)
        {
            // 加速力
            float acceleration = this->acceleration * elapsedFrame;

            // 空中なら加速力を減らす
            if (!IsGround()) acceleration *= 0.5f; // 半減

            // 加速処理  
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;


            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            // 最大速度の制限
            if (length > maxMoveSpeed)
            {
                DirectX::XMFLOAT3 normalized = {};
                length = 1 / length;
                normalized.x = velocity.x * length;
                normalized.z = velocity.z * length;

                if (velocity.x < 0.0f)
                {
                    velocity.x = normalized.x * maxMoveSpeed;
                }
                else
                {
                    velocity.x = normalized.x * maxMoveSpeed;
                }


                if (velocity.z < 0.0f)
                {
                    velocity.z = normalized.z * maxMoveSpeed;
                }
                else
                {
                    velocity.z = normalized.z * maxMoveSpeed;
                }
            }

        }

    }

    // 移動ベクトルのリセット
    moveVecX = 0.0f;
    moveVecZ = 0.0f;

}


void Character::UpdateHorizontalMove(float elapsedTime)
{

    // 水平速力
    float veclocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (veclocityLengthXZ > 0.0f)
    {
        float mx = velocity.x * elapsedTime;
        float mz = velocity.z * elapsedTime;

        DirectX::XMFLOAT3 start = { position.x,     position.y + stepOffset,    position.z };
        DirectX::XMFLOAT3 end = { position.x + mx,  position.y + stepOffset,    position.z + mz };

        DirectX::XMFLOAT3 start2 = { position.x,     position.y,    position.z };
        DirectX::XMFLOAT3 end2 = { position.x + mx,  position.y,    position.z + mz };

        HitResult hit, hit2;

        // レイキャスト2個目完成！
        //壁があれば
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // 壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);


            // 壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
            Normal = DirectX::XMVector3Normalize(Normal);
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            DirectX::XMFLOAT3 wall_vec;
            DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(Vec, DirectX::XMVectorMultiply(Dot, Normal)));



            start = { position.x,position.y + stepOffset,position.z };
            end = { position.x + wall_vec.x , position.y + stepOffset, position.z + wall_vec.z };

            // 壁ずり後の位置が壁に
            // めり込んでいれば
            if (StageManager::Instance().RayCast(start, end, hit))
            {
                // 更新しない

            }

            // めり込んでいなければ
            else
            {
                // 位置の更新
                position.x += wall_vec.x;
                position.z += wall_vec.z;
            }

            if (wall_hit == false)
            {
                AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Stop();
                AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Play(false);
            }

            wall_hit = true;

        }

        else if (StageManager::Instance().RayCast(start2, end2, hit2))
        {
            // 壁までのベクトル
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start2);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end2);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);


            // 壁の法線
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit2.normal);
            Normal = DirectX::XMVector3Normalize(Normal);
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            DirectX::XMFLOAT3 wall_vec;
            DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(Vec, DirectX::XMVectorMultiply(Dot, Normal)));



            start = { position.x,position.y,position.z };
            end = { position.x + wall_vec.x , position.y, position.z + wall_vec.z };

            // 壁ずり後の位置が壁に
            // めり込んでいれば
            if (StageManager::Instance().RayCast(start, end, hit))
            {
                // 更新しない

            }

            // めり込んでいなければ
            else
            {
                // 位置の更新
                position.x += wall_vec.x;
                position.z += wall_vec.z;
            }

            if (wall_hit == false)
            {
                AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Stop();
                AudioManager::Instance().GetAudio(Audio_INDEX::SE_WALLHIT)->Play(false);
            }

            wall_hit = true;
        }

        //壁がなければ
        else
        {
            // 通常の位置の更新
            position.x += mx;
            position.z += mz;

            wall_hit = false;
        }


    }

}

// 無敵時間更新
void Character::UpdateInvicibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}
