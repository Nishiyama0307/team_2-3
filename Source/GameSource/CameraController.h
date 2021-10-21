#pragma once

//#include <DirectXMath.h>
//#include "common.h"

enum class CAMERA
{
    NONE,
    PADCONTROL,
    NORMAL_TRACKING,
    TRANSLATION_TRACKING,
};



// �J��������N���X (�V���O���g��)

#if false
class CameraController
{
private:
    CameraController(){}
    ~CameraController(){}

public:
    static CameraController* Instance()
    {
        static CameraController instance;

        return &instance;
    }

    void Update(float elapsedTime, bool explaining = false);
    void init(const DirectX::XMFLOAT3 position_ = {},
        const DirectX::XMFLOAT3 target_ = {},
        const DirectX::XMFLOAT3 up_ = { 0,1,0 },
        const DirectX::XMFLOAT3 angle_ = { 0.718f,0,0 },
        const float range_ = 1.0f,
        const CAMERA camera_state_ = CAMERA::NONE);

public: // Get�֐�
    bool GetCameraShake() { return camera_shake; }
    float& GerRange() { return range; }
    DirectX::XMFLOAT3 GetPosition() { return this->position; }

public: // Set�֐�
    void Set(const DirectX::XMFLOAT3 position_, const DirectX::XMFLOAT3 target_, const DirectX::XMFLOAT3 up_);
    void SetTarget(const DirectX::XMFLOAT3& new_target_) { this->new_target = new_target_; }
    void SetPosition(const DirectX::XMFLOAT3& pos) { this->position = pos; }
    void SetCameraShake() { camera_shake = true; }
    void SetCameraBehavior(CAMERA next_camera);
    void SetRange(float range_);
    void SetCollision(bool flag_) { is_collision = flag_; }

private:
    void Collision();

private: //      �J�����̋���      //
    void Behavior(float elapsedTime, bool explaining = false);
    void PadControl(float elapsedTime, bool explaining);             // �J�����̒��ډ�]����
    //void NormalTracking(float elapsedTime);         // �ǂ�����
    //void TranslationTracking(float elapsedTime);    // ���s�ړ����Ȃ���ǂ�

private: //      �J�����Ɋ|�������    //
    void Shake(float elapsedTime);                  // �J�����V�F�C�N
    void ShakeXY();
    void ShakeInit();

private: // �ϐ�

    // �p���\��
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 target = {};
    DirectX::XMFLOAT3 up = {};
    DirectX::XMFLOAT3 angle = {};

    // �����_�ƃJ�����̋�����
    float range = 0.0f;

    // �J�����̋������
    CAMERA now_camera_state;
    CAMERA next_camera_state;
    bool is_collision = false;
    DirectX::XMFLOAT3 last_position = {};               // �؂�ւ��O�̃J�����ʒu

    // �J�����̌��ʗp
    bool camera_shake = false;                          // �J�����V�F�C�N
    const float DEFAULT_SHAKE_TIMER = 0.1f;
    float shake_timer = DEFAULT_SHAKE_TIMER;
    DirectX::XMFLOAT3 shake_power = {};

    DirectX::XMFLOAT3 new_position = {};
    DirectX::XMFLOAT3 new_target = {};
};
#endif

#include<DirectXMath.h>

//�J�����R���g���[���[
class CameraController
{
public:
    CameraController() {}
    ~CameraController() {}

    //�X�V����
    void Update(float elapsedTime);

    //�^�[�Q�b�g�ʒu��
    void SetTarget(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
    DirectX::XMFLOAT3  target = { 0, 0, 0 };
    DirectX::XMFLOAT3  angle = { 0, 0, 0 };
    float			   rollSpeed = DirectX::XMConvertToRadians(10);
    float			   range = 10.0f;

    float				maxAngleX = DirectX::XMConvertToRadians(45);
    float				minAngleX = DirectX::XMConvertToRadians(-45);
};