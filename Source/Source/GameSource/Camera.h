#pragma once

#include<DirectXMath.h>

//シングルトン
class Camera
{
private:
    Camera(){}
    ~Camera(){}

public:
    //唯一のインスタンス取得
    static Camera& Instance()
    {
        static Camera camera;
        return camera;
    }

    //指定方向を向く
    void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);

    //パーステクティブ設定(視点の設定)
    void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);

    const DirectX::XMFLOAT4X4& GetView() const { return view; }

    const DirectX::XMFLOAT4X4& GetProjection() const { return projection; }

    const DirectX::XMFLOAT3& GetEye() const { return eye;}
    const DirectX::XMFLOAT3& GetFocus() const { return focus;}
    const DirectX::XMFLOAT3& GetUp() const { return up;}
    const DirectX::XMFLOAT3& GetFront() const { return front;}
    const DirectX::XMFLOAT3& GetRight() const { return right;}

private:
    DirectX::XMFLOAT4X4 view = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    DirectX::XMFLOAT4X4 projection = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

    DirectX::XMFLOAT3 eye = {};
    DirectX::XMFLOAT3 focus = {};

    DirectX::XMFLOAT3 up = { 0,1,0 };
    DirectX::XMFLOAT3 front = { 0,0,1 };
    DirectX::XMFLOAT3 right = { 1,0,0 };
};
