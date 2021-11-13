#pragma once






// �R�����w�b�_�[(���ʂň����ϐ��A�}�N���̐錾)



// �}�N��
#define CAST_I(num)  static_cast<int>(num);
#define CAST_F(num)  static_cast<float>(num);


// �e���v���[�g�֐�
template <typename T>
inline void safe_delete(T*& p)
{
    if (p == nullptr) return;

    if (p)
    {
        delete (p);
        (p) = nullptr;
    }
}


// ���[�e�B���e�B�֐�
inline bool IsMaxUpper(const float num, const float num_max)
{
    return (num >= num_max);
}

inline bool ClampMax(float& num, const float num_max)
{
    if (IsMaxUpper(num, num_max) == false)return false;

    num = num_max;
    return true;
}