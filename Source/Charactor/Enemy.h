#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
class Player;


enum ENEMYTAG
{
	NORMAL_SWORD,
	STRONG_SWORD,
	NORMAL_SPEAR,
	STRONG_SPEAR,
};


class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override {}

	//virtual void Update(float elapsedTime) = 0;
	virtual void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos = {0,0,0}) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	void UpdateVelocity(float elapsedTime, int kind)override;

	// inhaled(�`�e���ŋz�����܂ꂽ)
	virtual DirectX::XMFLOAT3 inhaled() = 0;

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI(int i);

	// �v���C���[�ɑ΂��Ă̒ǔ�����
	void HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	// �s��X�V����
	void UpdateTransform();

	// �j��
	void Destroy();

	// ���S
	void OnDead();

	//// �ǐՂ��O�ꂽ
	//void OnNonTracking();

public: // Get�֐�
public: // Set�֐�
	void SetPlayerAddress(Player* player_) { player = player_; }
	virtual void SetInhaleParameter() = 0;

public: // length
	void SetLengthSq(float sq) { lengthSq = sq; }
	float GetLengthSq() const { return lengthSq; }

public:
	int enemy_tag;

	float lengthSq;

	bool Is_inhaling = false; // Is_inhaling(�z����) = �z�����܂�Ă邩�ǂ���
	Player* player = nullptr;


	DirectX::XMFLOAT3	target = { 0, 0, 0 };	
	DirectX::XMFLOAT3	direction = { 0,0,-1 };							// ���Ă������
	float				moveSpeed = 10.0f;										// �ړ����x
	float				turnSpeed = DirectX::XMConvertToRadians(180);	// ��]���x
	float				distance = 100;											// player��enemy�̋���
	float				Sensing_range = 50;										// ���m�͈�

	bool is_tracking_ = false;													// �ǐՂ��Ă邩�ǂ���
	bool is_detection_ = false;													// ���m�͈͂ɋ��邩�ǂ���
	bool is_group_behavior_ = false;
	
};