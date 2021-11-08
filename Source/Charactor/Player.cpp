
#include "Player.h"
#include "Graphics/Graphics.h"
#include "Input/Input.h"
#include "Camera.h"
#include "common.h"
#include "EnemyManager.h"
#include "collision.h"
#include "easy_math.h"
#include "Input/Mouse.h"

extern int attck_select_state;
extern bool f1;
// �R���X�g���N�^
Player::Player()
{
	model = new Model("Data/Model/player/player.mdl");

	scale.x = scale.y = scale.z = 1.0f;

	radius = 5.0f;

	inhale = new Inhale(this);

	position.y = scale.y + scale.y * 1.7f;

	height = 10.0f;

	//�A�j���[�V�����Đ�
	Idel_change();

	colstion_check1 = false;
	colstion_check2 = false;
	colstion_check3 = false;

	//��
	/*Audio& audio1 = Audio::Instance();
	Shot = audio1.LoadAudioSource("Data/Audio/Shot.wav");

	Audio& audio2 = Audio::Instance();
	Hit = audio2.LoadAudioSource("Data/Audio/Hit.wav");*/


	UpdateTransform();							// �I�u�W�F�N�g�s����X�V
	model->UpdateTransform(transform);			// ���f���s��X�V

}

// �f�X�g���N�^
Player::~Player()
{
	safe_delete(model);
	safe_delete(inhale);
}


DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	// ���͏����擾
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	// �J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX		= cameraRight.x;
	float cameraRightZ		= cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);
	if (cameraRightLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX		= cameraFront.x;
	float cameraFrontZ		= cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// �X�e�B�b�N�̐������͒l���J�����E�����ɔ��f���A
	// �X�e�B�b�N�̐������͒l���J�����O�����ɔ��f���A
	// �i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = cameraRightX * ax + cameraFrontX * ay;
	vec.z = cameraRightZ * ax + cameraFrontZ * ay;

	// Y�������ɂ͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;
}

// �X�V����
void Player::Update(float elapsedTime, bool explaining)
{
	if (explaining == false)
	{
		//Input(elapsedTime);							// ���͏���
		//UpdateVelocity(elapsedTime, KIND::PLAYER);	// ���͍X�V����
	}
	// ���͂��Ȃ��ɂ��� (���͂����C�ȉ�)
	else
	{

		velocity.x = 0.0f;
		velocity.z = 0.0f;

	}

	if (position.y < 0) jumpspeed = 40.0f;
	else jumpspeed = 20.0f;

	inhale->Update(elapsedTime);				// �|���@�@�\�̍X�V

	front = GetFront();

	//�A�j���[�V�����Đ�
	Mouse& mouse = Input::Instance().GetMouse();
	const MouseButton& anyButton = Mouse::BTN_LEFT;

	switch (state)
	{
		//�ҋ@
	case AnimeState::State_Idel:
		UpdateIdel(elapsedTime);
		break;

		//�U��1
	case AnimeState::State_Attack1:
		UpdateAttack1(elapsedTime);
		break;

		//�U��2
	case AnimeState::State_Attack2:
		UpdateAttack2(elapsedTime);
		break;

		//�U��3
	case AnimeState::State_Attack3:
		UpdateAttack3(elapsedTime);
		break;

		//���S
	case AnimeState::State_deth:
		UpdateDeth(elapsedTime);
		break;

		//����
	case AnimeState::State_Run:
		UpdateRun(elapsedTime);
		break;
	}

	// ���f���A�j���[�V�����X�V����
	model->UpdateAnimation(elapsedTime);


	// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
	CollisionPlayerVsEnemies();

	UpdateTransform();							// �I�u�W�F�N�g�s����X�V
	model->UpdateTransform(transform);			// ���f���s��X�V
}

void Player::Input(float elapsedTime)
{
	// �ړ����͏���
	InputMove(elapsedTime);

	// �W�����v���͏���
	InputJump();

	// �z�����݂̓��͏���
	//InputInhale();
}


void Player::InputMove(float elapsedTime)
{
	// �i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();

	// �ړ�����
	Move(moveVec.x, moveVec.z, moveSpeed);

	//Turn(elapsedTime, 0, 0, turnSpeed);
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);
}

// �W�����v���͏���
void Player::InputJump()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_SPACE)
	{
		if (jumpCount < jumpLimit)
		{
			Jump(jumpspeed);
			jumpCount++;
		}
	}
}

// ���n�����Ƃ��ɌĂ΂��
void Player::OnLanding()
{
	jumpCount = 0;
}

void Player::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;

	// �J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	// �ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	// �J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);
	if (cameraFrontLength > 0.0f)
	{
		// �P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = frontX * cameraFrontX + frontZ * cameraFrontZ; // ����
	float rot = 1.0 - dot; // ��]��

	// ���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	// �������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	if (rot > speed) rot = speed;


	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	// A.x * B.y - A.z * B.x
	float cross = frontX * cameraFrontZ - frontZ * cameraFrontX;

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����


	// ���E������s�����Ƃɂ���ĉ�]������I������
	if (cross < 0.0f)	// ���v���
	{
		angle.y += rot;
	}
	else				// �����v����
	{
		angle.y += -rot;
	}

}


void Player::InputInhale()
{
	inhale->InputInhale();

}


void Player::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	// �`�揈��
	shader->Draw(dc, model);
}

// �v���C���[�ƃG�l�~�[�Ƃ̏Փˏ���
void Player::CollisionPlayerVsEnemies()
{
	EnemyManager& enemyManager = EnemyManager::Instance();

	// �S�Ă̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);

		// ���Ƌ��̏Փˏ���
		//DirectX::XMFLOAT3 outPosition;
		//if (Collision::IntersectSqhereVsSqhere(
		//	position,
		//	radius,
		//	enemy->GetPosition(),
		//	enemy->GetRadius(),
		//	outPosition
		//))
		//{
		//	//�����o����̈ʒu�ݒ�
		//	enemy->SetPosition(outPosition);
		//}

		// �~���Ɖ~���̏Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition
		))
		{
			//DirectX::XMFLOAT3 up = { 0.0001f,1,0 };

			// �^�ォ�瓥�񂾂����ׂ�
			DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&position);
			DirectX::XMVECTOR E = DirectX::XMLoadFloat3(&enemy->GetPosition());
			DirectX::XMVECTOR V = DirectX::XMVectorSubtract(P, E);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(V);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3(&normal, N);

			/*float vy  = position.y - enemy->GetPosition().y;
			vy = sqrtf(vy * vy);
			if (vy > 0.8f)*/

			if (normal.y > 0.8f) // 0.8�͌o����
			{
				Jump(jumpspeed * 0.75f); // �W�����v��
				// �_���[�W��^����
				//enemy->ApplyDamage(1, 0.5f);
			}
			else
			{
				//�����o����̈ʒu�ݒ�
				enemy->SetPosition(outPosition);
			}
		}
	}
}

void Player::DrawDebugPrimitive()
{
	// �f�o�b�O�v���~�e�B�u�`��
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	
	// �Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(hit_position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	// �Փ˔���p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));


	debugRenderer->DrawSphere({ position.x,position.y + stepOffset,position.z }, 0.1f, DirectX::XMFLOAT4(0, 0, 0, 1));

	//�U���P
	if(colstion_check1)
		debugRenderer->DrawCylinder({ float3SUM(position,float3Scaling(GetFront(), radius * 2)) },
		radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//�U���Q
	if (colstion_check2)
		debugRenderer->DrawCylinder({ position.x,position.y,position.z},
			12.5, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//�U���R
	if (colstion_check3)
		debugRenderer->DrawCylinder({ float3SUM(position,float3Scaling(GetFront(), radius * 2)) },
			radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

	inhale->DebugRender();
}


void Player::DrawDebugGUI()
{
	// �f�o�b�O�pGUI�`��
}


void Player::AddImpact(const DirectX::XMFLOAT3 impact_)
{

	this->velocity.x += impact_.x;
	this->velocity.y += impact_.y;
	this->velocity.z += impact_.z;
}


////�A�j���[�V�����̃X�e�[�g�֌W
//�ҋ@�X�e�[�g��
void Player::Idel_change()
{
	state = AnimeState::State_Idel;
	model->PlayAnimation(Anim_Idel, true);
}

//�ҋ@�X�e�[�g�X�V
void Player::UpdateIdel(float elapsedTime)
{
	if (f1)
	{
		Input(elapsedTime);							// ���͏���
		UpdateVelocity(elapsedTime, KIND::PLAYER);	// ���͍X�V����

		//�U���I���ɂ���ăA�j���[�V�������ς��
		Mouse& mouse = Input::Instance().GetMouse();
		const MouseButton& anyButton = Mouse::BTN_LEFT;
		if (mouse.GetButtonDown() & anyButton)
		{
			switch (attck_select_state)
			{
			case 0:
				Attack1_change();
				break;
			case 1:
				Attack2_change();
				break;
			case 2:
				Attack3_change();
				break;
			}
		}
	}
	f1 = true;
}

//�U���X�e�[�g��  1
void Player::Attack1_change()
{
	animeTimer = 0;
	state = AnimeState::State_Attack1;
	model->PlayAnimation(Anim_Attack1, false);
}

//�U���X�e�[�g�X�V  1
void Player::UpdateAttack1(float elapsedTime)
{
	animeTimer++;
	//colstion_check1 = true;
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 attackPosition;
	//attackPosition.x = position.x;
	//attackPosition.y = position.y;
	//attackPosition.z = position.z + radius *2;

	attackPosition = { float3SUM(position,float3Scaling(GetFront(), radius * 2)) };
	if (animeTimer > 12.0f)colstion_check1 = true;
	if (animeTimer > 33.0f)colstion_check1 = false;

	if (colstion_check1)
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// �~���Ɖ~���̏Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			attackPosition, radius, height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition))
		{
			enemy->ApplyDamage(10, 5);
		}

	}

	if (!model->IsPlayAnimation())
	{

		colstion_check1 = false;
		Idel_change();
	}
}

//�U���X�e�[�g��  2
void Player::Attack2_change()
{
	animeTimer = 0;
	state = AnimeState::State_Attack2;
	model->PlayAnimation(Anim_Attack2, false);
}

//�U���X�e�[�g�X�V  2
void Player::UpdateAttack2(float elapsedTime)
{
	animeTimer++;
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 attackPosition;
	attackPosition.x = position.x;
	attackPosition.y = position.y;
	attackPosition.z = position.z;

	//attackPosition = { float3SUM(position,float3Scaling(GetFront(), radius * 2)) };

	if (animeTimer > 24.0f)colstion_check2 = true;
	if (animeTimer > 87.0f)colstion_check2 = false;

	if(colstion_check2)
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// �~���Ɖ~���̏Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			attackPosition, 12.5, height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition))
		{
			enemy->ApplyDamage(10, 5);
		}
	}
	if (!model->IsPlayAnimation())
	{
		colstion_check2 = false;
		Idel_change();
	}
}

//�U���X�e�[�g��  3
void Player::Attack3_change()
{
	animeTimer = 0;
	state = AnimeState::State_Attack3;
	model->PlayAnimation(Anim_Attack3, false);
}

//�U���X�e�[�g�X�V  3
void Player::UpdateAttack3(float elapsedTime)
{
	animeTimer++;
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	DirectX::XMFLOAT3 attackPosition;
	attackPosition = { float3SUM(position,float3Scaling(GetFront(), radius * 2)) };

	if(animeTimer > 135.98f)colstion_check3 = true;
	if(animeTimer > 148.98f)colstion_check3 = false;


	if(colstion_check3)
	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		// �~���Ɖ~���̏Փˏ���
		DirectX::XMFLOAT3 outPosition;
		if (Collision3D::IntersectCylinderVsCylinder(
			attackPosition, radius, height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition))
		{
			enemy->ApplyDamage(10, 5);
		}
	}
	if (!model->IsPlayAnimation())
	{
		colstion_check3 = false;
		Idel_change();
	}
}

//�f�X�X�e�[�g��
void Player::Deth_change()
{
	state = AnimeState::State_deth;
}
//�f�X�X�e�[�g�X�V
void Player::UpdateDeth(float elapsedTime)
{
	model->PlayAnimation(Anim_deth, false);
}

//����X�e�[�g��
void Player::Run_change()
{
	state = AnimeState::State_Run;
}
//����X�e�[�g�X�V
void Player::UpdateRun(float elapsedTime)
{
	Input(elapsedTime);							// ���͏���
	UpdateVelocity(elapsedTime, KIND::PLAYER);	// ���͍X�V����

	model->PlayAnimation(Anim_Run, true);
}