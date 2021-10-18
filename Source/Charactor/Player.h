#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Model.h"
#include "Character.h"
#include "inhale.h"
//#include "Audio/Audio.h"

//#define HIT_VOLUME (1.0f)
//#define SHOT_VOLUME (1.0f)

class Player : public Character
{
public:
	Player();
	~Player() override;

	void Update(float elapsedTime, bool explaining = false);
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	// �f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();

	// �f�o�b�O�pGUI�`��
	void DrawDebugGUI();

public:	// Get�֐�
	DirectX::XMFLOAT3 GetFront() { return ConvertToIdentityVec(transform._31, transform._32, transform._33); }
	DirectX::XMFLOAT3 GetRight() { return ConvertToIdentityVec(transform._11, transform._12, transform._13); }
	DirectX::XMFLOAT3 GetVelocity() { return velocity; }
	
public: // Set�֐�
	void AddImpact(const DirectX::XMFLOAT3 impact_);
	void ResetTransform()
	{
		position = { 0, scale.y + scale.y * 1.7f, 0 };
		angle = {};


		UpdateTransform();							// �I�u�W�F�N�g�s����X�V
		model->UpdateTransform(transform);			// ���f���s��X�V
	}

private:

	void Input(float elapsedTime);

	// �ړ����͒l����
	void InputMove(float elapsedTime);

	// �W�����v���͏���
	void InputJump();

	// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	void CollisionPlayerVsEnemies();

	void Turn(float elapsedTime, float vx, float vz, float speed)override;

	void InputInhale();

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
	int				jumpLimit = 1;

	Inhale* inhale = nullptr;				// �z�����݊֌W�̏����N���X

	//float	velocity_calc = 0;

public:
	/*std::unique_ptr<AudioSource> Shot;
	std::unique_ptr<AudioSource> Hit;*/

};