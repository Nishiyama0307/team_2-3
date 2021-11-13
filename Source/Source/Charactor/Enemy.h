#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
class Player;


constexpr float				kSensing_Range = 50;									// ���m�͈�
constexpr float				kAttack_Range = 10;										// �U���͈�
constexpr float				kAction_Range = 20;										// �s���͈�

enum ENEMYTAG
{
	NORMAL_SWORD,
	STRONG_SWORD,
	NORMAL_SPEAR,
	STRONG_SPEAR,
};

enum ANIMINDEX
{
	IDLE,
	WALK,
	RUN,
	ATTACK,
	DETH,
};

class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override {}

	//virtual void Update(float elapsedTime) = 0;
	virtual void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos = {0,0,0}) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	void UpdateVelocity(float elapsedTime, int kind, const DirectX::XMFLOAT3& playerPos);
	//void UpdateHorizontalMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// inhaled(�`�e���ŋz�����܂ꂽ)
	virtual DirectX::XMFLOAT3 inhaled() = 0;

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI(int i);

	// �v���C���[�ɑ΂��Ă̓���̍X�V
	void UpdateMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// �����_���Ȍx紈ړ�
	void RandomMove(float elapsedTime);

	// �v���C���[�ɑ΂��Ă̒ǔ�����
	void HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// �v���C���[�ɑ΂��Ă̍U��
	void AttackMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// �s��X�V����
	void UpdateTransform();

	// �j��
	void Destroy();

	// ���S
	void OnDead();

	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform._31, transform._32, transform._33); }

public: // Get�֐�
public: // Set�֐�
	void SetPlayerAddress(Player* player_) { player = player_; }
	virtual void SetInhaleParameter() = 0;
	void SetStageNum(int stage_num) { this->stage_num = stage_num; }

public: // length
	void SetLengthSq(float sq) { lengthSq = sq; }
	float GetLengthSq() const { return lengthSq; }

public:
	ENEMYTAG enemy_tag;

	float lengthSq;

	bool Is_inhaling = false; // Is_inhaling(�z����) = �z�����܂�Ă邩�ǂ���
	Player* player = nullptr;


	DirectX::XMFLOAT3	start_position = { 0, 0, 0 };					// �����ʒu
	DirectX::XMFLOAT3	target = { 0, 0, 0 };	
	DirectX::XMFLOAT3	direction = { 0,0,-1 };							// ���Ă������
	DirectX::XMFLOAT3	random_target = { 0,0,0 };							// ���Ă������
	float				moveSpeed = 10.0f;										// �ړ����x
	float				turnSpeed = DirectX::XMConvertToRadians(180);	// ��]���x
	float				distance = 100;											// player��enemy�̋���

	bool is_tracking_ = false;													// �ǐՂ��Ă邩�ǂ���
	bool is_detection_ = false;													// ���m�͈͂ɋ��邩�ǂ���
	bool is_group_behavior_ = false;											// �c�̍s�����Ă邩�ǂ���
	bool is_attacking_ = false;													// �U�����Ă邩�ǂ���
	bool is_walking = true;													// �����Ă��邩�ǂ���
	bool is_change_direction_ = false;											// ����������ς���邩�ǂ���
	bool is_action_range_ = true;												// �s���͈͂ɂ��邩�ǂ���

	int timer{ 0 };
	int random_direction{ 0 };
	int stage_num;

private:
	DirectX::XMFLOAT3 ConvertToIdentityVec(float x, float y, float z)
	{
		DirectX::XMFLOAT3 identity_vec = {};
		DirectX::XMStoreFloat3(&identity_vec, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z))));
		return identity_vec;
	}
	
};