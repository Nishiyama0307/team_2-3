#pragma once

#include <DirectXMath.h>



struct Parameter
{
	DirectX::XMFLOAT3 scaling_value = {0,0,0};		// �z�����񂾕��ɂ���ā{or�|
	float enough_total_scale_value = 0.0f;			// �z�����߂�Œ჉�C��
	float can_be_digestion_total_scale_value = 0.0f;// �����ł���Œ჉�C��
};


// �L�����N�^�[	
class Character
{
public:
	Character() {}
	virtual ~Character() {}

	// �s��X�V����
	void UpdateTransform();

	const DirectX::XMFLOAT3& GetPosition() const { return position; }

	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

	const DirectX::XMFLOAT3& GetAngle() const { return angle; }

	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

	const DirectX::XMFLOAT3& GetScale() const { return scale; }

	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	// ���a�擾
	float GetRadius() const { return radius; }

	// �����擾
	float GetHeight() const { return height; }

	// �n�ʂɐڒn���Ă��邩
	bool IsGround() const { return isGround; }

	// �_���[�W��^����
	bool ApplyDamage(int damage, float invincibleTime);

	// �Ռ���^����
	void AddImpulse(const DirectX::XMFLOAT3& impulse);
protected:

	// �ړ�����
	void Move(float vx, float vz, float speed);

	// ���񏈗�
	virtual void Turn(float elapsedTime, float vx, float vz, float speed);

	// �W�����v����
	void Jump(float speed);

	// ���͏����X�V
	virtual void UpdateVelocity(float elapsedTime, int kind);


protected:
	// �������͂̍X�V
	void UpdateVerticalVelocity(float elapsedFrame, int kind);
	// �����ړ��̍X�V
	void UpdateVerticalMove(float elapsedTime, int kind = 0);
	// �������͂̍X�V
	void UpdateHorizontalVelocity(float elapsedFrame);
	// �����ړ��̍X�V
	void UpdateHorizontalMove(float elapsedTime);

	// ���G���ԍX�V
	void UpdateInvicibleTimer(float elapsedTime);

	// ���n�������ɌĂ΂��
	virtual void OnLanding() {}

	// �_���[�W���󂯂��Ƃ��ɌĂ΂��
	virtual void OnDamaged() {}

	// ���S�����Ƃ��ɌĂ΂��
	virtual void OnDead() {}

protected:
	DirectX::XMFLOAT3	position = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0,0,0 };
	DirectX::XMFLOAT3	scale = { 1,1,1 };
	DirectX::XMFLOAT4X4	transform = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	DirectX::XMFLOAT3 foot_pos = { 0,0,0 };
	
	float				radius = 0.5f;

	float				gravity = -1.0f;
	DirectX::XMFLOAT3	velocity = { 0,0,0 };

	bool				isGround = false;

	int					health = 5;

	float				height = 2.0f;

	float				invincibleTimer = 1.0f; // ���G����

	float friction = 0.5f; // ���C��
	float acceleration = 1.0f;
	float maxMoveSpeed = 5.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;

	float				airControl = 0.3f;

	float stepOffset = 1.0f;

	enum KIND{
		PLAYER,		// �v���C���[
		NORMAL_ENEMY, // �m�[�}�����u
		BOMB_ENEMY,	// �f�����b�g���u
		RARE_ENEMY,	// ���A���u
	};

	bool wall_hit = false;

public:
	Parameter par;
};