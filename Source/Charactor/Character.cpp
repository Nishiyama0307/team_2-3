#include "Character.h"
#include "collision.h"
#include "stageManager.h"

#include "audioManager.h"


// �s��X�V����
void Character::UpdateTransform()
{
	// �X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// ��]�s����쐬
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	// �ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// �R�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	// �v�Z�������[���h�s������o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

// �_���[�W��^����
bool Character::ApplyDamage(int damage, float invincibleTime)
{
    // �_���[�W��0�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
    if (damage == 0) return false;

    // ���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health == 0) return false;

    // �_���[�W����
    if (invincibleTimer < 0.1f)
    {
        health -= damage;
        invincibleTimer = invincibleTime;
    }

    // ���S�ʒm
    if (health == 0)
    {
        OnDead();
    }
    else
        // �_���[�W�ʒm
    {
        OnDamaged();
    }

    // ���N��Ԃ��ύX�����ꍇ��true��Ԃ�

    return true;
}

// �Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
    // ���͂ɗ͂�������
    velocity.x += impulse.x;
    velocity.y += impulse.y;
    velocity.z += impulse.z;
}

// �ړ�����
void Character::Move(float vx, float vz, float speed)
{
    // �ړ������̃x�N�g���ݒ�
    moveVecX = vx;
    moveVecZ = vz;

    // �ő呬�x�̐ݒ�
    maxMoveSpeed = speed;
}

// ���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	float length = sqrtf(vx * vx + vz * vz);
	if (length < 0.0001f) return;

	// �i�s�x�N�g����P�ʃx�N�g����
	vx /= length;
	vz /= length;

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	// A.x * B.x + A.z * B.z
	float dot = frontX * vx + frontZ * vz; // ����
	float rot = 1.0 - dot; // �␳�l

	// ���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	if (rot < speed) speed = rot;


	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	// A.x * B.y - A.z * B.x
	float cross = frontX * vz - frontZ * vx;

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����


	// ���E������s�����Ƃɂ���č��E��]��I������
	if (cross <= 0.0f) // �E�ɐ���
	{
		angle.y += speed;
	}
	else if (cross > 0.0f) // ���ɐ���
	{
		angle.y -= speed;
	}

}

// �W�����v����
void Character::Jump(float speed)
{
	// ������̗͂�ݒ�
	velocity.y = speed;
}

// ���͏����X�V
void Character::UpdateVelocity(float elapsedTime, int kind)
{
    //�@�o�߃t���[��
    float elapsedFrame = 60.0f * elapsedTime;

    // ��������
    UpdateVerticalVelocity(elapsedFrame, kind);

    // ��������
    UpdateHorizontalVelocity(elapsedFrame);


    // �����ړ�
    UpdateVerticalMove(elapsedTime);

    // �����ړ�
    UpdateHorizontalMove(elapsedTime);
}

// �������͍X�V����
void Character::UpdateVerticalVelocity(float elapsedFrame, int kind)
{
    // �d�͏���
    velocity.y += gravity * elapsedFrame;
}

// �����ړ��X�V����
void Character::UpdateVerticalMove(float elapsedTime, int kind)
{

    //// �ړ�����
    //position.y += velocity.y * elapsedTime;

    //float offset = 0.0f;
    ////if (kind == 0) offset = scale.y * 1.7f;
    ////if (kind > 0) 
    //offset = -scale.y * 0.3f;

    //foot_pos.y = position.y - scale.y - offset;

    //// �n�ʔ���
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

    // ���������̈ړ���
    float my = velocity.y * elapsedTime;

    // ������
    if (my < 0.0f)
    {
        // ���C�̊J�n�ʒu�͑�����菭����
        DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
        // ���C�̏I�_�ʒu�͈ړ���̈ʒu
        DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };

        // ���C�L���X�g�ɂ��n�ʔ���
        HitResult hit;
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // �n�ʂɐڒn���Ă���
            position.y = hit.position.y;

            // ���n����
            if (!isGround)
            {
                OnLanding();
            }
            isGround = true;
            velocity.y = 0.0f;
        }
        else
        {
            // �󒆂ɕ����Ă���
            position.y += my;
            isGround = false;
        }
    }
    // �㏸��
    else if (my > 0.0f)
    {
        position.y += my;
        isGround = false;
    }
}


void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
    // XZ�̗͂̋���(����)���Z�o
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

    // ���͂����鎞 (���C����������)
    if (length > 0.0f)
    {

        // ���C��
        float friction = this->friction * elapsedFrame;

        // �󒆂Ȃ疀�C�͂����炷
        if (!IsGround()) friction *= 0.7f; // �R����

        // ���C�ɂ�錸������ (���͂����C�ȏ�)
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

        // ���͂��Ȃ��ɂ��� (���͂����C�ȉ�)
        else
        {

            velocity.x = 0.0f;
            velocity.z = 0.0f;

        }

    }


    // ���͂��ő呬�x�ȓ��̂Ƃ��@(��������������)
    if (length <= maxMoveSpeed)
    {
        // �ړ��x�N�g���̎Z�o
        float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);

        //�ړ��x�N�g�������鎞 (��������������)
        if (moveVecLength > 0.0f)
        {
            // ������
            float acceleration = this->acceleration * elapsedFrame;

            // �󒆂Ȃ�����͂����炷
            if (!IsGround()) acceleration *= 0.5f; // ����

            // ��������  
            velocity.x += moveVecX * acceleration;
            velocity.z += moveVecZ * acceleration;


            float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);

            // �ő呬�x�̐���
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

    // �ړ��x�N�g���̃��Z�b�g
    moveVecX = 0.0f;
    moveVecZ = 0.0f;

}


void Character::UpdateHorizontalMove(float elapsedTime)
{

    // ��������
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

        // ���C�L���X�g2�ڊ����I
        //�ǂ������
        if (StageManager::Instance().RayCast(start, end, hit))
        {
            // �ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);


            // �ǂ̖@��
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);
            Normal = DirectX::XMVector3Normalize(Normal);
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            DirectX::XMFLOAT3 wall_vec;
            DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(Vec, DirectX::XMVectorMultiply(Dot, Normal)));



            start = { position.x,position.y + stepOffset,position.z };
            end = { position.x + wall_vec.x , position.y + stepOffset, position.z + wall_vec.z };

            // �ǂ����̈ʒu���ǂ�
            // �߂荞��ł����
            if (StageManager::Instance().RayCast(start, end, hit))
            {
                // �X�V���Ȃ�

            }

            // �߂荞��ł��Ȃ����
            else
            {
                // �ʒu�̍X�V
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
            // �ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start2);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end2);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);


            // �ǂ̖@��
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit2.normal);
            Normal = DirectX::XMVector3Normalize(Normal);
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

            DirectX::XMFLOAT3 wall_vec;
            DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(Vec, DirectX::XMVectorMultiply(Dot, Normal)));



            start = { position.x,position.y,position.z };
            end = { position.x + wall_vec.x , position.y, position.z + wall_vec.z };

            // �ǂ����̈ʒu���ǂ�
            // �߂荞��ł����
            if (StageManager::Instance().RayCast(start, end, hit))
            {
                // �X�V���Ȃ�

            }

            // �߂荞��ł��Ȃ����
            else
            {
                // �ʒu�̍X�V
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

        //�ǂ��Ȃ����
        else
        {
            // �ʏ�̈ʒu�̍X�V
            position.x += mx;
            position.z += mz;

            wall_hit = false;
        }


    }

}

// ���G���ԍX�V
void Character::UpdateInvicibleTimer(float elapsedTime)
{
    if (invincibleTimer > 0.0f)
    {
        invincibleTimer -= elapsedTime;
    }
}
