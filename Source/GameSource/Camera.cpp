#include "Camera.h"

void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    DirectX::XMVECTOR Eye   = DirectX::XMLoadFloat3(&eye);      // �J�����̎��_�i�ʒu�j
    DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);    // �J�����̒����_�i�^�[�Q�b�g�j
    DirectX::XMVECTOR Up    = DirectX::XMLoadFloat3(&up);       // �J�����̏����
    DirectX::XMMATRIX View  = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
    DirectX::XMStoreFloat4x4(&view, View);

    // ���[���h�s��ɖ߂����߂Ƀr���[���t�s��
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;

    this->eye = eye;
    this->focus = focus;
    //this->up = up;
}

void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
    //��p�A��ʔ䗦�A�N���b�v��������v���W�F�N�V�����s��̍쐬
    DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
    DirectX::XMStoreFloat4x4(&projection, Projection);
}