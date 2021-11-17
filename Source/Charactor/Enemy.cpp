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


// �f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	
	// �����蔻��̑傫��
	radius = 1.0f;

	// ���A�����ݒ�
	height = 6.0f;
	
	// �Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(hit_position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	// �Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	if(attack)
		debugRenderer->DrawCylinder({ float3SUM(position,float3Scaling(GetFront(), radius * 4)) },
			radius, height, DirectX::XMFLOAT4(1, 0, 0, 1));

	switch (enemy_tag)
	{
	case NORMAL_SWORD:
		// �s���͈�
		if(is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(1, 0, 0, 1));
		// ���m�͈�
		if(is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(1, 0, 0, 1));
		// �U���ڍs�͈�
		if(is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(1, 0, 0, 1));
		break;
	case STRONG_SWORD:
		// �s���͈�
		if (is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(0, 0, 1, 1));
		// ���m�͈�
		if (is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(0, 0, 1, 1));
		// �U���ڍs�͈�
		if (is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(0, 0, 1, 1));
		break;
	case NORMAL_SPEAR:
		// �s���͈�
		if (is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(0, 1, 0, 1));
		// ���m�͈�
		if (is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(0, 1, 0, 1));
		// �U���ڍs�͈�
		if (is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(0, 1, 0, 1));
		break;
	case STRONG_SPEAR:
		// �s���͈�
		if (is_walking) debugRenderer->DrawCylinder(start_position, kAction_Range, 3, DirectX::XMFLOAT4(1, 1, 0, 1));
		// ���m�͈�
		if (is_tracking_ == false) debugRenderer->DrawCylinder(position, kSensing_Range, 3, DirectX::XMFLOAT4(1, 1, 0, 1));
		// �U���ڍs�͈�
		if (is_tracking_ && is_attacking_ == false) debugRenderer->DrawCylinder(position, kAttack_Range, 3, DirectX::XMFLOAT4(1, 1, 0, 1));
		break;
	}
}

// �f�o�b�O�pGUI�`��
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


	// �����ړ�
	//UpdateHorizontalMove(elapsedTime, playerPos);

	//if (!wall_hit) UpdateMove(elapsedTime, playerPos);


}
//
//void Enemy::UpdateHorizontalMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
//{
//	float Speed;
//	// �ړ�
//	if(model->GetIndex() != ANIMINDEX::RUN) Speed = this->moveSpeed * elapsedTime;
//	else Speed = (this->moveSpeed * elapsedTime) / 2;
//
//	// ��������
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
//		// ���C�L���X�g2�ڊ����I
//		//�ǂ������
//		if (StageManager::Instance().RayCast(start, end, hit))
//		{
//			// �ǂ܂ł̃x�N�g��
//			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
//			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
//			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//
//			// �ǂ̖@��
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
//			// �ǂ����̈ʒu���ǂ�
//			// �߂荞��ł����
//			if (StageManager::Instance().RayCast(start, end, hit))
//			{
//				// �X�V���Ȃ�
//				position.x += -wall_vec.x;
//				position.z += -wall_vec.z;
//			}
//
//			// �߂荞��ł��Ȃ����
//			else
//			{
//				// �ʒu�̍X�V
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
//			// �ǂ܂ł̃x�N�g��
//			DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start2);
//			DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end2);
//			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
//
//
//			// �ǂ̖@��
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
//			// �ǂ����̈ʒu���ǂ�
//			// �߂荞��ł����
//			if (StageManager::Instance().RayCast(start, end, hit))
//			{
//				// �X�V���Ȃ�
//				position.x += -wall_vec.x;
//				position.z += -wall_vec.z;
//			}
//
//			// �߂荞��ł��Ȃ����
//			else
//			{
//				// �ʒu�̍X�V
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
//		//�ǂ��Ȃ����
//		else
//		{
//			// �ʏ�̈ʒu�̍X�V
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

// �s��X�V����
void Enemy::UpdateTransform()
{
	DirectX::XMVECTOR Front, Up, Right;

	// �O�x�N�g�����Z�o
	Front = DirectX::XMLoadFloat3(&direction);
	Front = DirectX::XMVector3Normalize(Front);

	// ���̏�x�N�g�����Z�o
	Up = DirectX::XMVectorSet(0.0001f, 1, 0, 0);// ������Ƃ��炷({0, 1, 0}�͂悭�g������)
	Up = DirectX::XMVector3Normalize(Up);

	// �E�x�N�g�����Z�o(���̏ꍇ������1�ɂȂ��Ă���)
	Right = DirectX::XMVector3Cross(Up, Front);

	// ��x�N�g�����Z�o
	Up = DirectX::XMVector3Cross(Front, Right);

	// �v�Z���ʂ����o��
	DirectX::XMFLOAT3 right, up, front;
	DirectX::XMStoreFloat3(&right, Right);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&front, Front);

	// �Z�o�������x�N�g������s����쐬
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

	// ���˕���
	this->direction = front;
}

// �v���C���[�ɑ΂��Ă̓���̍X�V
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

// �����_���Ȍx紈ړ�
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
			constexpr float zero_avoidance{ 0.0001f }; // �[���x�N�g�����p

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
			case 0: // �O
				// ����target�ɂ���Ċ��ɑO�������ĕ����Ă��邩��킴�킴target�̐ݒ������K�v���Ȃ�
				//random_target.x = 0;
				//random_target.z = target_range;
				break;
			case 1: // �E
				random_target.x = target_range;
				random_target.z = 0;
				break;
			case 2: // ��
				random_target.x = 0;
				random_target.z = -target_range;
				break;
			case 3: // ��
				random_target.x = -target_range;
				random_target.z = 0;
				break;
			}
			target = { this->position.x + random_target.x, this->position.y, this->position.z + random_target.z };
			timer++;
		}

		// ����
		{
			float turnSpeed = this->turnSpeed * elapsedTime;

			// �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
			DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
			DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

			// �[���x�N�g���łȂ��Ȃ��]����
			DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
			float lengthSq;
			DirectX::XMStoreFloat(&lengthSq, LengthSq);


			if (lengthSq > 0.00001f)
			{
				// �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
				Vec = DirectX::XMVector3Normalize(Vec);


				// �����Ă�������x�N�g�����Z�o
				DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

				// �O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

				float dot;
				DirectX::XMStoreFloat(&dot, Dot);

				// �Q�̒P�ʃx�N�g���̊p�x���������ق�
				// 1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
				float rot = 1.0f - dot; // 0~2�܂ł̒l�ɂȂ��Ă���
				if (rot > turnSpeed && rot < 1.0f) // ���������Ă���ƃz�[�~���O���Ȃ�
				{
					rot = turnSpeed;
				}

				// ��]�p�x������Ȃ��]����������
				if ((rot) > 0.0001f/*&& fabsf(rot) < 1.0f*/ || /*(rot) > 0.0001f &&*/ is_action_range_ == false) // ���������Ă���ƃz�[�~���O���Ȃ� 
				{
					// ��]�����Z�o
					DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
					// ��]���Ɖ�]�ʂ����]�s����Z�o
					DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
					// ���݂̍s�����]������
					DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
					Transform = DirectX::XMMatrixMultiply(Transform, Rotation);
					// ��]��̑O�����������o���A�P�ʃx�N�g��������
					Direction = DirectX::XMVector3Normalize(Transform.r[2]);
					DirectX::XMStoreFloat3(&direction, Direction);

				}
			}
		}

		// �ړ�
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

// �v���C���[�ɑ΂��Ă̒ǔ�����
void Enemy::HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
{
	if (distance < kSensing_Range) is_detection_ = true;
	else is_detection_ = false;

	if (is_detection_ == false && is_tracking_) // �͈͊O�ŒǐՒ��Ȃ�ʂ�
	{
		is_tracking_ = false; // ���m�͈͊O�ɓ�����ꂽ��ǐՂ���߂�
		model->PlayAnimation(ANIMINDEX::IDLE, true);
	}

	// ����
	{
		float turnSpeed = this->turnSpeed * elapsedTime;

		if (is_detection_) target = playerPos;

		// �^�[�Q�b�g�܂ł̃x�N�g�����Z�o
		DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);
		DirectX::XMVECTOR Target = DirectX::XMLoadFloat3(&target);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Target, Position);

		// �[���x�N�g���łȂ��Ȃ��]����
		DirectX::XMVECTOR LengthSq = DirectX::XMVector3LengthSq(Vec);
		float lengthSq;
		DirectX::XMStoreFloat(&lengthSq, LengthSq);


		if (lengthSq > 0.00001f)
		{
			// �^�[�Q�b�g�܂ł̃x�N�g����P�ʃx�N�g����
			Vec = DirectX::XMVector3Normalize(Vec);


			// �����Ă�������x�N�g�����Z�o
			DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);

			// �O�������x�N�g���ƃ^�[�Q�b�g�܂ł̃x�N�g���̓��ρi�p�x�j���Z�o
			DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Direction, Vec);

			float dot;
			DirectX::XMStoreFloat(&dot, Dot);

			// �Q�̒P�ʃx�N�g���̊p�x���������ق�
			// 1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
			float rot = 1.0f - dot; // 0~2�܂ł̒l�ɂȂ��Ă���
			if (rot > turnSpeed && rot < 1.0f) // ���������Ă���ƃz�[�~���O���Ȃ�
			{
				rot = turnSpeed;
			}

			// ��]�p�x������Ȃ��]����������
			if (((is_detection_  && (rot) > 0.0001f && fabsf(rot) < 1.0f) || is_group_behavior_)) // ���m�͈͊O���A���������Ă���ƃz�[�~���O���Ȃ� �܂��͒c�̍s�����Ȃ�ʂ�
			{
				// ��]�����Z�o
				DirectX::XMVECTOR Axis = DirectX::XMVector3Cross(Direction, Vec);
				// ��]���Ɖ�]�ʂ����]�s����Z�o
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(Axis, rot);
				// ���݂̍s�����]������
				DirectX::XMMATRIX Transform = DirectX::XMLoadFloat4x4(&transform);
				Transform = DirectX::XMMatrixMultiply(Transform, Rotation);
				// ��]��̑O�����������o���A�P�ʃx�N�g��������
				Direction = DirectX::XMVector3Normalize(Transform.r[2]);
				DirectX::XMStoreFloat3(&direction, Direction);

				if (is_detection_ && is_tracking_ == false) // �͈͓��ŒǐՒ��łȂ���Βʂ�
				{
					is_tracking_ = true; // �O���ɋ�����ǐՂ��J�n����
					is_group_behavior_ = false;
					target = playerPos;
					model->PlayAnimation(ANIMINDEX::RUN, true); // ���郂�[�V����
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

// �v���C���[�ɑ΂��Ă̍U��
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
		if (animTimer > 38.28f /*5*/) //��0.6�b 
		{
			attack = true;
		}
		//if (model->GetIndex() == ANIMINDEX::ATTACK)
		//if (!model->IsPlayAnimation()) //����if��ʂ��Ă��Ȃ�->���̕�����Ȃ��c
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

// �j��
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}


// ���S
void Enemy::OnDead()
{
	model->PlayAnimation(ANIMINDEX::DETH, false);
	is_dead_ = true;
}

// ���n�����Ƃ��ɌĂ΂��
void Enemy::OnLanding()
{
	if(/*Only_once &&*/ stage_num == 1) // ��x�����ʂ�
	{
		start_position.y = position.y;
		//Only_once = false;
	}
}