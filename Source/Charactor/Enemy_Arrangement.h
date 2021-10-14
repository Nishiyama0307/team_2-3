#pragma once

#include "Charactor/Enemy.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/NormalEnemy.h"
#include "Charactor/BombEnemy.h"
#include "Charactor/RareEnemy.h"

using namespace std;
#include <string>
#include <fstream>
#include <time.h>

class Block
{
public:
    int num;
};

class Enemy_Arrangement
{
public:

    //------< �萔 >------------------------------------------------------------
    static const int CHIP_NUM_X = 500;       // �}�b�v�̉������̃`�b�v��
    static const int CHIP_NUM_Y = 500;       // �}�b�v�̏c�����̃`�b�v��
    static const int CHIP_SIZE = 2;       // %���Z�q���g�p���邽��int�^���g�p����
    static const int NORMAL_ENEMY_NUM = 170;       // normal_enemy�̐�
    static const int BOMB_ENEMY_NUM = 108;       // normal_enemy�̐�

private:
    //------< �ϐ� >------------------------------------------------------------

    Block Arrangement[CHIP_NUM_Y][CHIP_NUM_X];                   // �G�̔z�u�f�[�^
    int normal_enemy_limit_num = 30;
    int bomb_enemy_limit_num = 20;
    int normal_enemy_num[3] = {0};
    int bomb_enemy_num[3] = {0};
    int random_num = 0; 

    float random_scale = 1;
    int random_angle = 0;

    int normal_random = 0;
    bool bomb_random = false;


public:
    Enemy_Arrangement() {};
    ~Enemy_Arrangement() {};

    void random_scaling(int Arrangement_num);
    void random_angling();

    enum csv_file_num
    {
        TUTORIAL_NORMAL,
        TUTORIAL_BOMB,
        GAME,
    };

    enum enemy_size
    {
        SMALL_SIZE,
        MEDIUM_SIZE,
        LARGE_SIZE,
    };

    void enemy_produce(csv_file_num csv_file = GAME);
};

void csv_load(Block data[][Enemy_Arrangement::CHIP_NUM_X], const char* filename);