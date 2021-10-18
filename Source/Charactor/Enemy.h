#pragma once

#include "Graphics/Shader.h"
#include "Character.h"
class Player;


enum ENEMYTAG
{
	NORMAL,
	BOMB,
	RARE,
};


class Enemy : public Character
{
public:
	Enemy(){}
	~Enemy() override {}

	virtual void Update(float elapsedTime) = 0;
	virtual void Render(ID3D11DeviceContext* context, Shader* shader) = 0;

	void UpdateVelocity(float elapsedTime, int kind)override;

	// inhaled(�`�e���ŋz�����܂ꂽ)
	virtual DirectX::XMFLOAT3 inhaled() = 0;

	// �f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();

	// �j��
	void Destroy();

	// �����蔻��̈ʒu�X�V
	void HitPositionUpdate();

public: // Get�֐�
	float EnoughTotalScaleValue() { return par.enough_total_scale_value; }
	float CanBeDigestionTotalScaleValue() { return par.can_be_digestion_total_scale_value; }

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

	
};