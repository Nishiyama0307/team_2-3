

#include "fileIO.h"
#include <iostream>
#include <fstream>




void FileIO::Open(const char* filename, FileData& data_)
{
    std::ifstream fin(filename, std::ios::in | std::ios::binary);
    //  �t�@�C�����J��
    //  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��


    if (!fin) return;
    //  �t�@�C�����J���Ȃ������Ƃ��̑΍�


    int value;

    while (true) {  //�t�@�C���̍Ō�܂ő�����

        fin.read((char*)&value, sizeof(int));

        if (fin.eof()) break;

        data_.data_array.emplace_back(value);
        //������ł͂Ȃ��f�[�^��ǂ݂���
    }

    fin.close();  //�t�@�C�������
}


void FileIO::Write(const char* filename, FileData& data_)
{
    std::ofstream fout;
    fout.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    //  �t�@�C�����J��
    //  ios::out �͏������ݐ�p�i�ȗ��j
    //  ios::binary �̓o�C�i���`���ŏo�́i�ȗ�����ƃA�X�L�[�`���ŏo�́j
    //  ios::trunc�̓t�@�C����V�K�쐬�i�ȗ��j
    //  ios::add�ɂ���ƒǋL�ɂȂ�

    if (!fout) return;
    //  �t�@�C�����J���Ȃ������Ƃ��̑΍�

    for (size_t i = 0; i < data_.data_array.size(); ++i) {

        fout.write((char*)&data_.data_array.at(i), sizeof(int));
        //������ł͂Ȃ��f�[�^����������
        // �usizeof( int )�v�o�C�g�́uchar *�v�|�C���^�udata_array.at(i)�v���f�[�^�Ƃ��ďo��
    }

    fout.close();  //�t�@�C�������
}