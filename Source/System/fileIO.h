#pragma once

#include <vector>
struct FileData;



// ファイルの入出力クラス

class FileIO
{
public: // Get関数
    // filenameのファイルからFileDataに情報を読み込む
    static void Open(const char* filename, FileData& data_);

public: // Set関数
    // FileDataの情報をfilenameのファイルに書き込む
    static void Write(const char* filename, FileData& data_);
};


// ファイル入出力用のデータ構造体

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