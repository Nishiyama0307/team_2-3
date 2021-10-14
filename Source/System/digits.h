#pragma once

#include <vector>
#include <math.h>



// ���l�N���X ���e���v���[�g�N���X

template <typename T>
class Digits
{
public:
    Digits(T default_start_digits_/*�����l*/, size_t max_digits_place_/*�ő包��*/) : default_start_digits(default_start_digits_), max_digits_place(max_digits_place_) {};
    virtual ~Digits() { digit = 0; };

protected: // Get�֐�
    T GetDigit() { return digit; }

protected: // Set�֐�
    void Reset()
    {
        digit = default_start_digits;
    }

    void Add(const T add)
    {
        if (add < 0) return;

        digit += add;
    }

    void Subtract(const T subtract)
    {
        if (subtract > 0) return;

        digit += subtract;
    }

protected:
    // �����Ƃɐ��l�𕪉�
    void SliceDigits(T digits_, std::vector<T>& digits_place)
    {
        // i = 0 �Ȃ�P�̈�, i = 1 �Ȃ�P�O�̈� ...
        for (size_t i = digits_place.size(); i > 0; --i)
        {
            size_t index = i - 1;
            float f_index = static_cast<float>(index);

            int place = static_cast<int>(static_cast<float>(digits_) * pow(0.1f, f_index));
            digits_place.at(index) = static_cast<T>(place);
            digits_ += static_cast<T>(-digits_place.at(index) * pow(10, f_index));
        }

        if (static_cast<int>(digits_) <= 0) return;
        // �v�Z���ʂ��O�łȂ���΃G���[
        assert(false);
    }

protected: // �萔
    const T default_start_digits;       // �����l
    const size_t max_digits_place;      // �ő包��

protected: // �ϐ�
    T digit = default_start_digits;     // ���l

};