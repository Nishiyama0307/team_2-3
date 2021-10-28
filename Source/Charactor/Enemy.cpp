#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Player.h"

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
}

// �f�o�b�O�pGUI�`��
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


// �v���C���[�ɑ΂��Ă̒ǔ�����
void Enemy::HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos)
{
	distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&playerPos))));

	if (distance < Sensing_range) is_detection_ = true;
	else is_detection_ = false;

	if (is_tracking_) is_group_behavior_ = false;

	

	if (is_detection_ == false && is_tracking_) // �͈͊O�ŒǐՒ��Ȃ�ʂ�
	{
		is_tracking_ = false; // ���m�͈͊O�ɓ�����ꂽ��ǐՂ���߂�
		model->PlayAnimation(0, true);
	}

	// ����
	{
		float turnSpeed = this->turnSpeed * elapsedTime;

		if(is_detection_) target = playerPos;

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
			if (((is_detection_ || is_group_behavior_) && (rot) > 0.0001f && fabsf(rot) < 1.0f) ) // ���m�͈͊O���A���������Ă���ƃz�[�~���O���Ȃ� �܂��͒c�̍s�����Ȃ�ʂ�
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

				// �ړ�
				if (is_tracking_ || is_group_behavior_) // �ǐՒ����c�̍s�����݈̂ړ�
				{
					float moveSpeed = this->moveSpeed * elapsedTime;
					position.x += direction.x * moveSpeed;
					position.y += direction.y * moveSpeed;
					position.z += direction.z * moveSpeed;
				}

				if (is_detection_ && is_tracking_ == false) // �͈͓��ŒǐՒ��łȂ���Βʂ�
				{
					is_tracking_ = true; // �O���ɋ�����ǐՂ��J�n����
					is_group_behavior_ = false;
					target = playerPos;
					model->PlayAnimation(2, true); // ���郂�[�V����
				}
			}
		}
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
	model->PlayAnimation(1, true);
}

//// �ǐՂ��O�ꂽ
//void Enemy::OnNonTracking()
//{
//	is_tracking_ = false; // ���m�͈͊O�ɓ�����ꂽ��ǐՂ���߂�
//}