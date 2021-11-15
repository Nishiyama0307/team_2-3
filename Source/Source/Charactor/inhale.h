#pragma once

#include "Actor/transform.h"
class Player;


// �z�����݃N���X �� Player�N���X�Ɉˑ�

class Inhale
{
public:
    Inhale(Player* player_);
    ~Inhale() {};

public:
    void Update(float elapsedTime);
    void DebugRender();

    void InputInhale();

public: // Get�֐�
    float GetHitRadius() { return radius; }
    bool IsDuringInhale() { return is_during_inhale; }

public: // Set�֐�

private:
    void UpdateNozzlePosition();                                        // �m�Y���̈ʒu�̍X�V�p
    void Collision();                                                   // �����蔻��
    void KnockBack(DirectX::XMFLOAT3 pos_a, DirectX::XMFLOAT3 pos_b);   // �m�b�N�o�b�N����


private:
    Player* player = nullptr;           // �v���C���[�̏������p����p
    bool is_during_inhale = false;      // �z���Ă��邩
    Transform nozzle = {};              // �p���\���悤�̕ϐ��Q
    float radius = 1.0f;                // �����蔻��(��)�̔��a
};
