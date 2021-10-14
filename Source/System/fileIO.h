#pragma once

#include <vector>
struct FileData;



// �t�@�C���̓��o�̓N���X

class FileIO
{
public: // Get�֐�
    // filename�̃t�@�C������FileData�ɏ���ǂݍ���
    static void Open(const char* filename, FileData& data_);

public: // Set�֐�
    // FileData�̏���filename�̃t�@�C���ɏ�������
    static void Write(const char* filename, FileData& data_);
};


// �t�@�C�����o�͗p�̃f�[�^�\����

struct FileData
{
    std::vector<int> data_array = {};

    void SetData(const int data_)
    {
        data_array.emplace_back(data_);
    }

    size_t Size()
    {
        return data_array.size();
    }
};