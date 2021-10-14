

#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "stageManager.h"
#include "collision.h"



void CameraController::Update(float elapsedTime, bool explaining)
{
    // �J���������̐؂�ւ��̗L��
    if (now_camera_state != next_camera_state)
    {
        // �؂�ւ�
        now_camera_state = next_camera_state;
    
        // �O��̃J�����ʒu��ۑ�
        last_position = position;
    }

    // �J�����̋���
    Behavior(elapsedTime, explaining);

    // �����蔻��
    //Collision();
    
    // �J�����V�F�C�N
    Shake(elapsedTime);

    // �n�`�Ƃ̓����蔻����s��
    HitResult	hitResult;
    if (StageManager::Instance().RayCast(new_target, new_position, hitResult))
    {
        DirectX::XMVECTOR	p = DirectX::XMLoadFloat3(&hitResult.position);
        DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
        p = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(4), cuv, p);
        DirectX::XMStoreFloat3(&new_position, p);
    }


    // ���X�ɖڕW�ɋ߂Â���
    static	constexpr	float	Speed = 1.0f / 8.0f;
    position.x += (new_position.x - position.x) * Speed;
    position.y += (new_position.y - position.y) * Speed;
    position.z += (new_position.z - position.z) * Speed;
    target.x += (new_target.x - target.x) * Speed;
    target.y += (new_target.y - target.y) * Speed;
    target.z += (new_target.z - target.z) * Speed;


    Camera::Instance().SetLookAt(position, target, up);

}


void CameraController::init(const DirectX::XMFLOAT3 position_, const DirectX::XMFLOAT3 target_, const DirectX::XMFLOAT3 up_, const DirectX::XMFLOAT3 angle_, const float range_, const CAMERA camera_state_)
{
    position = position_;
    target = target_;
    up = up_;
    angle = angle_;

    range = range_;

    DirectX::XMFLOAT3 last_position = {};

    camera_shake = false;
    shake_timer = DEFAULT_SHAKE_TIMER;
    shake_power = {};

    now_camera_state = camera_state_;
}


void CameraController::Collision()
{
    if (is_collision == false) return;


    constexpr float wall_size = 500.0f;
    constexpr float wall_thickness = 50.0f;

    constexpr float camera_radius = 3.0f;

    // �����o���ʒu�ƒ����_���狁�܂钷�����ƃJ������range��ӂƂ��Ďg���A�J�����̍����ƈʒu���Z�o������
    auto ClampPosition = [](const DirectX::XMFLOAT2 out, const DirectX::XMFLOAT2 target, const float range) -> DirectX::XMFLOAT3
    {
        float length;
        // xz���ʁ@�Ōv�Z������ӂ̒���
        DirectX::XMStoreFloat(&length, DirectX::XMVector2Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&out), DirectX::XMLoadFloat2(&target))));

        // �ʒu�̎Z�o
        DirectX::XMFLOAT3 pos = {};
        pos.x = out.x;
        pos.y += sqrtf(range * range - length * length);   // �Εӂ̂Q��@- ��ӂ̂Q�� = �����̂Q��
        pos.z = out.y;

        return pos;
    };

    DirectX::XMFLOAT2 out_position = {};

    // x,z = max,0
    if (Collision2D::RectVsCircleAndExtrusion({ wall_size * 0.5f + wall_thickness * 0.5f ,0 }, { wall_thickness,wall_size }, { position.x,position.z }, camera_radius, out_position))
    {
        /*DirectX::XMFLOAT3 n_vec_old;
        DirectX::XMStoreFloat3(&n_vec_old, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&target))));*/

        position = ClampPosition(out_position, DirectX::XMFLOAT2(target.x, target.z), range);

        float range_new;
        DirectX::XMStoreFloat(&range_new, DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&target))));
        range = range_new;
        
        /*
        // �����Ƃ̊p�x�𒲐߂��悤�Ƃ������ǁA���ς��狁�܂����p�x���ǂ������Ē��߂ł���̂��킩��Ȃ������I
        DirectX::XMFLOAT3 n_vec_new;
        DirectX::XMStoreFloat3(&n_vec_new, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&target))));

        float dot;  // �傫�����P�̃x�N�g�����m�̓��ς́@cos�� = a�Eb�@�ƂȂ�̂ŁA�� = arccos(a�Eb)
        DirectX::XMStoreFloat(&dot, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&n_vec_old), DirectX::XMLoadFloat3(&n_vec_new)));
        */

        out_position = {};
    }

    // x,z = 0,max
    if (Collision2D::RectVsCircleAndExtrusion({ 0,wall_size * 0.5f + wall_thickness * 0.5f }, { wall_size,wall_thickness }, { position.x,position.z }, camera_radius, out_position))
    {
        position = ClampPosition(out_position, DirectX::XMFLOAT2(target.x, target.z), range);

        float range_new;
        DirectX::XMStoreFloat(&range_new, DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&target))));
        range = range_new;

        out_position = {};
    }

    // x,z = min,0
    if (Collision2D::RectVsCircleAndExtrusion({ -wall_size * 0.5f - wall_thickness * 0.5f ,0 }, { wall_thickness,wall_size }, { position.x,position.z }, camera_radius, out_position))
    {
        position = ClampPosition(out_position, DirectX::XMFLOAT2(target.x, target.z), range);

        float range_new;
        DirectX::XMStoreFloat(&range_new, DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&target))));
        range = range_new;

        out_position = {};
    }

    // x,z = 0,min
    if (Collision2D::RectVsCircleAndExtrusion({ 0,-wall_size * 0.5f - wall_thickness * 0.5f }, { wall_size,wall_thickness }, { position.x,position.z }, camera_radius, out_position))
    {
        position = ClampPosition(out_position, DirectX::XMFLOAT2(target.x, target.z), range);

        float range_new;
        DirectX::XMStoreFloat(&range_new, DirectX::XMVector3Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&target))));
        range = range_new;

        out_position = {};
    }
}


void CameraController::PadControl(float elapsedTime, bool explaining = false)
{

    constexpr float rollspeed = DirectX::XMConvertToRadians(90);
    float speed = rollspeed * elapsedTime;


    //��]����
    if(explaining == false)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        float ax = gamePad.GetAxisRX();
        float ay = gamePad.GetAxisRY();

        angle.x += ay * speed;
        angle.y += ax * speed;
    }


    constexpr float maxAngle = DirectX::XMConvertToRadians(45);
    constexpr float minAngle = DirectX::XMConvertToRadians(0);

    //X���J������]�̐���
    if (angle.x > maxAngle) angle.x = maxAngle;
    if (angle.x < minAngle) angle.x = minAngle;

    //X���J������]�̐���
    if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
    if (angle.y > DirectX::XM_PI)  angle.y -= DirectX::XM_2PI;


    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    new_position.x = target.x - front.x * range;
    new_position.y = target.y - front.y * range;
    new_position.z = target.z - front.z * range;
}


void CameraController::Behavior(float elapsedTime, bool explaining)
{
    // ���R���
    if (now_camera_state == CAMERA::NONE) {}

    // �E�X�e�B�b�N�ŉ�]����
    if (now_camera_state == CAMERA::PADCONTROL)
        PadControl(elapsedTime, explaining);

    // �ʏ�ǔ�
    if (now_camera_state == CAMERA::NORMAL_TRACKING)
        NormalTracking(elapsedTime);

    // ���s�ړ��ǔ�
    if (now_camera_state == CAMERA::TRANSLATION_TRACKING)
        TranslationTracking(elapsedTime);
}


void CameraController::NormalTracking(float elapsedTime)
{
    // ����]����p���s�����蒼���Ă���̂ŁA������ς������ꍇ�͎��p�x��ύX����
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);


    // �O��ʒu����range�����ꂽ�ʒu
    new_position.x = last_position.x;
    new_position.y = last_position.y;
    new_position.z = last_position.z;


    //  ���O�񂪂Ȃ���Ό��_
    if (last_position.x == 0.0f && last_position.y == 0.0f && last_position.z == 0.0f)
    {
        new_position.x = 0.0f - front.x * range;
        new_position.y = 0.0f - front.y * range;
        new_position.z = 0.0f - front.z * range;
    }


}


void CameraController::TranslationTracking(float elapsedTime)
{
    // ����]����p���s�����蒼���Ă���̂ŁA������ς������ꍇ�͎��p�x��ύX����
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //DirectX::XMFLOAT3 eye;
    // �^�[�Q�b�g����range�����ꂽ�ʒu
    new_position.x = target.x - front.x * range;
    new_position.y = target.y - front.y * range;
    new_position.z = target.z - front.z * range;
}


void CameraController::Shake(float elapsedTime)
{
    if (camera_shake == false) return;

    ShakeXY();

    shake_timer += -1 * elapsedTime;

    if (shake_timer < 0.0f) ShakeInit();


    // �V�F�C�N���̉��Z     ����{ 0
    new_position.x += shake_power.x;
    new_position.y += shake_power.y;
    //position.z += shake_power.z;
}


void CameraController::ShakeXY()
{
    constexpr float shake_value_range_X = 15.0f;    // x�� 15�̕� (-7.5f ~ 7.5f)
    constexpr float shake_value_range_Y = 5.0f;     // y��  5�̕� (-2.5f ~ 2.5f)

    // 0x7fff = �萔 RAND_MAX
    float rand_max = 1.0f / (float)0x7fff;

    // �����l�ŗ�������
    shake_power.x = (float)rand() * rand_max * shake_value_range_X - shake_value_range_X * 0.5f;
    shake_power.y = (float)rand() * rand_max * shake_value_range_Y - shake_value_range_Y * 0.5f;

}


void CameraController::ShakeInit()
{
    camera_shake = false;
    shake_timer = DEFAULT_SHAKE_TIMER;
    shake_power = {};
}


void CameraController::Set(const DirectX::XMFLOAT3 position_, const DirectX::XMFLOAT3 target_, const DirectX::XMFLOAT3 up_)
{
    position = position_;
    target = target_;
    up = up_;
}


void CameraController::SetCameraBehavior(CAMERA next_camera)
{
    // �J���������ɕύX���Ȃ����
    if (now_camera_state == next_camera) return;


    next_camera_state = next_camera;
}

void CameraController::SetRange(float range_)
{
    if (range_ < 0) return;

    range = range_;
}