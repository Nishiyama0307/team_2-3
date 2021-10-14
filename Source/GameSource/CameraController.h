#pragma once

#include <DirectXMath.h>
#include "common.h"

enum class CAMERA
{
    NONE,
    PADCONTROL,
    NORMAL_TRACKING,
    TRANSLATION_TRACKING,
};



// カメラ操作クラス (シングルトン)

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

public: // Get関数
    bool GetCameraShake() { return camera_shake; }
    float& GerRange() { return range; }
    DirectX::XMFLOAT3 GetPosition() { return this->position; }

public: // Set関数
    void Set(const DirectX::XMFLOAT3 position_, const DirectX::XMFLOAT3 target_, const DirectX::XMFLOAT3 up_);
    void SetTarget(const DirectX::XMFLOAT3& new_target_) { this->new_target = new_target_; }
    void SetPosition(const DirectX::XMFLOAT3& pos) { this->position = pos; }
    void SetCameraShake() { camera_shake = true; }
    void SetCameraBehavior(CAMERA next_camera);
    void SetRange(float range_);
    void SetCollision(bool flag_) { is_collision = flag_; }

private:
    void Collision();

private: //      カメラの挙動      //
    void Behavior(float elapsedTime, bool explaining = false);
    void PadControl(float elapsedTime, bool explaining);             // カメラの直接回転操作
    void NormalTracking(float elapsedTime);         // 追うだけ
    void TranslationTracking(float elapsedTime);    // 平行移動しながら追う

private: //      カメラに掛ける効果    //
    void Shake(float elapsedTime);                  // カメラシェイク
    void ShakeXY();
    void ShakeInit();

private: // 変数

    // 姿勢表現
    DirectX::XMFLOAT3 position = {};
    DirectX::XMFLOAT3 target = {};
    DirectX::XMFLOAT3 up = {};
    DirectX::XMFLOAT3 angle = {};

    // 注視点とカメラの距離感
    float range = 0.0f;

    // カメラの挙動状態
    CAMERA now_camera_state;
    CAMERA next_camera_state;
    bool is_collision = false;
    DirectX::XMFLOAT3 last_position = {};               // 切り替え前のカメラ位置

    // カメラの効果用
    bool camera_shake = false;                          // カメラシェイク
    const float DEFAULT_SHAKE_TIMER = 0.1f;
    float shake_timer = DEFAULT_SHAKE_TIMER;
    DirectX::XMFLOAT3 shake_power = {};

    DirectX::XMFLOAT3 new_position = {};
    DirectX::XMFLOAT3 new_target = {};
};