#pragma once






// コモンヘッダー(共通で扱う変数、マクロの宣言)



// マクロ
#define CAST_I(num)  static_cast<int>(num);
#define CAST_F(num)  static_cast<float>(num);


// テンプレート関数
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


// ユーティリティ関数
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