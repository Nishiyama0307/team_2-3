#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "scene/pause.h"

//#include "Audio/Audio.h"

//#define HIT_VOLUME (1.0f)
//#define SHOT_VOLUME (1.0f)

class Player : public Character
{
public:
	Player();
	~Player() override;

	void Update(float elapsedTime, int stage_num, bool explaining = false);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

public:	// Get�֐�
	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform._31, transform._32, transform._33); }
	DirectX::XMFLOAT3 GetRight() { return ConvertToIdentityVec(transform._11, transform._12, transform._13); }
	DirectX::XMFLOAT3 GetVelocity() { return velocity; }
	Model* GetModel() { return model; }

	DirectX::XMFLOAT3 front;
public: // Set�֐�
	void AddImpact(const DirectX::XMFLOAT3 impact_);
	void ResetTransform()
	{
		position = { 0, 0, -160.0f };
		angle = {};


		UpdateTransform();							// �I�u�W�F�N�g�s����X�V
		model->UpdateTransform(transform);			// ���f���s��X�V
	}

private:

	void Input(float elapsedTime);

	// �ړ����͒l����
	void InputMove(float elapsedTime);
	//bool InputMove(float elapsedTime);

	// �W�����v���͏���
	void InputJump();

	// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();

	void Turn(float elapsedTime, float vx, float vz, float speed)override;

	void Stage1_Gimmick();

	void OnDead();

protected:
	// ���n�����Ƃ��ɌĂ΂��
	void OnLanding() override;

private:
	DirectX::XMFLOAT3 GetMoveVec() const;
	DirectX::XMFLOAT3 ConvertToIdentityVec(float x, float y, float z)
	{
		DirectX::XMFLOAT3 identity_vec = {};
		DirectX::XMStoreFloat3(&identity_vec, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z))));
		return identity_vec;
	}

private:
	Model* model = nullptr;

	float	moveSpeed = 25.0f;

	float	turnSpeed = DirectX::XMConvertToRadians(720);

	float			jumpspeed = 20.0f;

	int				jumpCount = 0;

#if _DEBUG
	int				jumpLimit = 100;
#else
	int				jumpLimit = 1;
#endif

	//float	velocity_calc = 0;

public:
	/*std::unique_ptr<AudioSource> Shot;
	std::unique_ptr<AudioSource> Hit;*/

private:
	//�A�j���[�V�����J��
	enum AnimationPlayer
	{
		Anim_Idel,
		Anim_walk,
		Anim_Run,
		Anim_deth,
		Anim_Damage,
		Anim_Attack1,
		Anim_Attack2,
		Anim_Attack3,
	};
	int anime_state = 0;
	enum class AnimeState
	{
		State_Idel,
		State_walk,
		State_Run,
		State_deth,
		State_Attack1,
		State_Attack2,
		State_Attack3,
		State_Damage,
	};
	AnimeState state = AnimeState::State_Idel;

	float animeTimer = 0;
	bool colstion_check1 = false;
	bool colstion_check2 = false;
	bool colstion_check3 = false;

	//�ҋ@�X�e�[�g��
	void Idel_change();
	//�ҋ@�X�e�[�g�X�V
	void UpdateIdel(float elapsedTime);

	//�U������
	void attack_Branch();
	//�A�[�}�[
	bool armor = false;
	//1�U���X�e�[�g��
	void Attack1_change();
	//1�U���X�e�[�g�X�V
	void UpdateAttack1(float elapsedTime);
	//2�U���X�e�[�g��
	void Attack2_change();
	//2�U���X�e�[�g�X�V
	void UpdateAttack2(float elapsedTime);
	//3�U���X�e�[�g��
	void Attack3_change();
	//3�U���X�e�[�g�X�V
	void UpdateAttack3(float elapsedTime);

	//�_���[�W�X�e�[�g
	void Damage_change();
	//�_���[�W�X�V
	void UpdateDamage(float elapsedTime);
	float damage = 0;

	//���񂾃X�e�[�g��
	//void Deth_change();
	////���񂾃X�e�[�g�X�V
	//void UpdateDeth(float elapsedTime);


	//�����X�e�[�g��
	void Walk_change();
	//�����X�e�[�g�X�V
	void UpdateWalk(float elapsedTime);

	//����X�e�[�g��
	void Run_change();
	//����X�e�[�g�X�V
	void UpdateRun(float elapsedTime);

	//�G�̍U������炤�֐�
	void EnemyAttckHit();

	//�[���x�N�g�����`�F�b�N
	bool zeroVec = false;
	//�V�t�g�L�[���������̃`�F�b�N
	bool isbuttn = false;

public:
	//shift�_�b�V���̃X�^�~�i�Q�[�W
	float stamina_limit{ 512 };
	float stamina = stamina_limit;

	int kstamina_recovery{ 70 };

	bool is_stamina_recovering{ false };

	//�}�O�}�Ŏ��S������
	bool magumaDeath = false;

	//private:
		//std::unique_ptr<Pause> pause = nullptr;
};