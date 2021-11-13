

//#include "CameraController.h"
//#include "Camera.h"
//#include "Input/Input.h"
//#include "stageManager.h"
//#include "collision.h"


#if false
void CameraController::Update(float elapsedTime, bool explaining)
{
    // カメラ挙動の切り替えの有無
    if (now_camera_state != next_camera_state)
    {
        // 切り替え
        now_camera_state = next_camera_state;
    
        // 前回のカメラ位置を保存
        last_position = position;
    }

    // カメラの挙動
    Behavior(elapsedTime, explaining);

    // 当たり判定
    //Collision();
    
    // カメラシェイク
    Shake(elapsedTime);

    // 地形との当たり判定を行う
    HitResult	hitResult;
    if (StageManager::Instance().RayCast(new_target, new_position, hitResult))
    {
        DirectX::XMVECTOR	p = DirectX::XMLoadFloat3(&hitResult.position);
        DirectX::XMVECTOR	cuv = DirectX::XMVectorSet(0, 1, 0, 0);
        p = DirectX::XMVectorMultiplyAdd(DirectX::XMVectorReplicate(4), cuv, p);
        DirectX::XMStoreFloat3(&new_position, p);
    }


    // 徐々に目標に近づける
    static	constexpr	float	Speed = 1.0f / 8.0f;
    //position.x += (new_position.x - position.x) * Speed;
    //position.y += (new_position.y - position.y) * Speed;
    //position.z += (new_position.z - position.z) * Speed;
    position.x = (new_position.x);
    position.y = (new_position.y);
    position.z = (new_position.z);
    //target.x += (new_target.x - target.x) * Speed;
    //target.y += (new_target.y - target.y) * Speed;
    //target.z += (new_target.z - target.z) * Speed;
    target.x = (new_target.x );
    target.y = (new_target.y );
    target.z = (new_target.z );


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

    // 押し出し位置と注視点から求まる長さをとカメラのrangeを辺として使い、カメラの高さと位置を算出したい
    auto ClampPosition = [](const DirectX::XMFLOAT2 out, const DirectX::XMFLOAT2 target, const float range) -> DirectX::XMFLOAT3
    {
        float length;
        // xz平面　で計算した底辺の長さ
        DirectX::XMStoreFloat(&length, DirectX::XMVector2Length(DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&out), DirectX::XMLoadFloat2(&target))));

        // 位置の算出
        DirectX::XMFLOAT3 pos = {};
        pos.x = out.x;
        pos.y += sqrtf(range * range - length * length);   // 斜辺の２乗　- 底辺の２乗 = 高さの２乗
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
        // 軸ごとの角度を調節しようとしたけど、内積から求まった角度をどうつかって調節できるのかわからなかった！
        DirectX::XMFLOAT3 n_vec_new;
        DirectX::XMStoreFloat3(&n_vec_new, DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&position), DirectX::XMLoadFloat3(&target))));

        float dot;  // 大きさが１のベクトル同士の内積は　cosΘ = a・b　となるので、Θ = arccos(a・b)
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

    constexpr float rollspeed = DirectX::XMConvertToRadians(10);
    float speed = rollspeed * elapsedTime;


    //回転操作
    Mouse& mouse = Input::Instance().GetMouse();
    if (explaining == false)
    {
        GamePad& gamePad = Input::Instance().GetGamePad();
        //float ax = gamePad.GetAxisRX();
        //float ay = gamePad.GetAxisRY();

        float ax = mouse.GetPositionX();
        float ay = mouse.GetPositionY();

        angle.x += ay * speed;
        angle.y += ax * speed;
    }


    constexpr float maxAngle = DirectX::XMConvertToRadians(45);
    constexpr float minAngle = DirectX::XMConvertToRadians(0);

    //X軸カメラ回転の制限
    //if (angle.x > maxAngle) angle.x = maxAngle;
    //if (angle.x < minAngle) angle.x = minAngle;
    //
    ////X軸カメラ回転の制限
    //if (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
    //if (angle.y > DirectX::XM_PI)  angle.y -= DirectX::XM_2PI;


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
    // 自由状態
    if (now_camera_state == CAMERA::NONE) {}

    // 右スティックで回転操作
    if (now_camera_state == CAMERA::PADCONTROL)
        PadControl(elapsedTime, explaining);

    // 通常追尾
    //if (now_camera_state == CAMERA::NORMAL_TRACKING)
       // NormalTracking(elapsedTime);

    // 平行移動追尾
    //if (now_camera_state == CAMERA::TRANSLATION_TRACKING)
        //TranslationTracking(elapsedTime);
}

#if false
void CameraController::NormalTracking(float elapsedTime)
{
    // 軸回転から姿勢行列を作り直しているので、向きを変えたい場合は軸角度を変更する
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);


    // 前回位置からrange分離れた位置
    new_position.x = last_position.x;
    new_position.y = last_position.y;
    new_position.z = last_position.z;


    //  ※前回がなければ原点
    if (last_position.x == 0.0f && last_position.y == 0.0f && last_position.z == 0.0f)
    {
        new_position.x = 0.0f - front.x * range;
        new_position.y = 0.0f - front.y * range;
        new_position.z = 0.0f - front.z * range;
    }


}


void CameraController::TranslationTracking(float elapsedTime)
{
    // 軸回転から姿勢行列を作り直しているので、向きを変えたい場合は軸角度を変更する
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);


    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //DirectX::XMFLOAT3 eye;
    // ターゲットからrange分離れた位置
    new_position.x = target.x - front.x * range;
    new_position.y = target.y - front.y * range;
    new_position.z = target.z - front.z * range;
}
#endif


void CameraController::Shake(float elapsedTime)
{
    if (camera_shake == false) return;

    ShakeXY();

    shake_timer += -1 * elapsedTime;

    if (shake_timer < 0.0f) ShakeInit();


    // シェイク分の加算     ※基本 0
    new_position.x += shake_power.x;
    new_position.y += shake_power.y;
    //position.z += shake_power.z;
}


void CameraController::ShakeXY()
{
    constexpr float shake_value_range_X = 15.0f;    // x軸 15の幅 (-7.5f ~ 7.5f)
    constexpr float shake_value_range_Y = 5.0f;     // y軸  5の幅 (-2.5f ~ 2.5f)

    // 0x7fff = 定数 RAND_MAX
    float rand_max = 1.0f / (float)0x7fff;

    // 実数値で乱数生成
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
    // カメラ挙動に変更がなければ
    if (now_camera_state == next_camera) return;


    next_camera_state = next_camera;
}

void CameraController::SetRange(float range_)
{
    if (range_ < 0) return;

    range = range_;
}
#endif

#include "CameraController.h"
#include "Camera.h"
#include "Input/Input.h"
#include "Input/Mouse.h"
#include "C_function.h"
//float ax = 0;
//float ay = 0;

#include<Windows.h>

//更新処理
void CameraController::Update(float elapsdTime)
{
    ShowCursor(false);

    GamePad& gamepad = Input::Instance().GetGamePad();

    Mouse& mouse = Input::Instance().GetMouse();

    const MouseButton& anyButton = VK_LBUTTON;

#if false
    float mouseSpeedX, mouseSpeedY, nowMousePosX, nowMousePosY, oldMousePosX, oldMousePosY;
    nowMousePosX = mouse.GetPositionX();
    nowMousePosY = mouse.GetPositionY();
    oldMousePosX = mouse.GetOldPositionX();
    oldMousePosY = mouse.GetOldPositionY();
    float speed = rollSpeed * elapsdTime;

    mouseSpeedX = oldMousePosX - nowMousePosX;
    mouseSpeedY = oldMousePosY - nowMousePosY;

    //angle.x = mouseSpeedX * speed;
    //if (angle.x > 360) angle.x = 0;
    //if (angle.x < 0) angle.x = 360;

    //angle.y = mouseSpeedY * speed;
    //if (angle.y > 360) angle.y = 0;
    //if (angle.y < 0) angle.y = 360;
#endif

    float mouseSpeedX, mouseSpeedY, nowMousePosX, nowMousePosY, oldMousePosX, oldMousePosY;
    nowMousePosX = mouse.GetPositionX();
    nowMousePosY = mouse.GetPositionY();
    oldMousePosX = mouse.GetOldPositionX();
    oldMousePosY = mouse.GetOldPositionY();
    // カメラの回転速度
    //左右回転
    if (oldMousePosX > nowMousePosX)
    {
        rollSpeed = DirectX::XMConvertToRadians(float(oldMousePosX - nowMousePosX));
        float speed = rollSpeed * elapsdTime;
        if (!setpos)
        {
            this->angle.y -= speed * 7.0f;
        }
    }
    if (oldMousePosX < nowMousePosX)
    {
        rollSpeed = DirectX::XMConvertToRadians(float(nowMousePosX - oldMousePosX));
        float speed = rollSpeed * elapsdTime;
        if (!setpos)
        {
            this->angle.y += speed * 7.0f;
        }
    }
    //上下回転
    if (oldMousePosY > nowMousePosY)
    {
        rollSpeed = DirectX::XMConvertToRadians(float(oldMousePosY - nowMousePosY));
        float speed = rollSpeed * elapsdTime;
        if (!setpos)
        {
            this->angle.x -= speed * 7.0f;
        }
    }
    if (oldMousePosY < nowMousePosY)
    {
        rollSpeed = DirectX::XMConvertToRadians(float(nowMousePosY - oldMousePosY));
        float speed = rollSpeed * elapsdTime;
        if (!setpos)
        {
            this->angle.x += speed * 7.0f;
        }
    }
    setpos = false;
    //移動量が0の時真ん中に戻す
    if (0 == oldMousePosX - nowMousePosX)
    {
        ::SetCursorPos(960, 540);
        setpos = true;
    }
    //結局マウスの移動量が端につくと無くなるのでは?
    if (nowMousePosX < 10)
    {
        if (0 == oldMousePosX - nowMousePosX)
        {
            ::SetCursorPos(960, 540);
            setpos = true;
        }
    }
    if (nowMousePosX > 1910)
    {
        if (0 == oldMousePosX - nowMousePosX)
        {
            ::SetCursorPos(960, 540);
            setpos = true;
        }
    }

    //上下のカメラ回転を制限
    if (angle.x > maxAngleX)
    	angle.x = maxAngleX ;
    if (angle.x < minAngleX)
    	angle.x = minAngleX;
   
#if false
    //if (mouse.GetPositionX() < 10)
    //{
    //    SetCursorPos(960, 540);
    //    mouse.SetOldPositionX(960);
    //    mouse.SetPositionX(960);
    //}
    //if (mouse.GetPositionX() >= 1700)
    //{
    //    SetCursorPos(940, 540);
    //    mouse.SetOldPositionX(940);
    //    mouse.SetPositionX(940);
    //}
    //else if (mouse.GetPositionX() >= 940)
    //{
    //
    //}
    //else if (mouse.GetPositionX() <= 940)
    //{
    //   
    //}
    //else if (mouse.GetPositionX() <= 100)
    //{
    //    SetCursorPos(980, 540);
    //    mouse.SetOldPositionX(980);
    //    mouse.SetPositionX(980);
    //}

    // X軸のカメラ回転を制限	
    //if (this->angle.x >= maxAngleX)angle.x = maxAngleX;
    //if (this->angle.x <= minAngleX)angle.x = minAngleX;
    //// Y軸の回転値を-3.14~3.14に収まるようにする
    //if (angle.y < -DirectX::XM_PI)
    //{
    //    angle.y += DirectX::XM_2PI;
    //}
    //if (angle.y > DirectX::XM_PI)
    //{
    //    angle.y -= DirectX::XM_2PI;
    //}
#endif

#if false
    //if (mouse.GetButton() & anyButton)
    //{
        //ax = mouse.GetPositionX();
        //ay = mouse.GetPositionY();
    //}

    //float ax = gamepad.GetAxisRX();
    //float ay = gamepad.GetAxisRY();

    //float ax = mouse.GetPositionX();
    //float ay = mouse.GetPositionY();

   /* float ax2 = ax - 10;
    float ay2 = ay - 10;

    if (ax2 > 1910) ax += 5;
    if (ax2 < 20) ax -= 5;
    if (ay2 > 1060) ay += 5;
    if (ay2 < 20) ay -= 5;*/

    //カメラの回転速度
    float speed = rollSpeed * elapsdTime;

    //スティックの入力値に合わせてx軸とｙ軸を回転
    //angle.x = ay * speed;
    //angle.y = ax * speed;

    //X軸のカメラ回転を制限
    //if (angle.x > maxAngleX)
    //	angle.x = maxAngleX ;
    //if (angle.x < minAngleX)
    //	angle.x = minAngleX;
    //
    ////y軸の回転値を-3.14~3.14に収まるようにする
    //if (angle.y < -DirectX::XM_PI)
    //{
    //	angle.y += DirectX::XM_2PI;
    //}
    //if (angle.y > DirectX::XM_PI)
    //{
    //	angle.y -= DirectX::XM_2PI;
    //}
#endif

    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //回転行列から前方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注意点から後ろベクトル方向にい一定距離離れたカメラ視点を求める
    DirectX::XMFLOAT3 eye;
    eye.x = target.x - front.x * range;
    eye.y = target.y - front.y * range;
    eye.z = target.z - front.z * range;

    //カメラの視点と注意点を設定
    Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

}
