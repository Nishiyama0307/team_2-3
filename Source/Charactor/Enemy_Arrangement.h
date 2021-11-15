#pragma once

#include "Charactor/Enemy.h"
#include "Charactor/EnemyManager.h"
//#include "Charactor/NormalEnemy.h"
//#include "Charactor/BombEnemy.h"
//#include "Charactor/RareEnemy.h"
#include "Charactor/NormalSword.h"
#include "Charactor/StrongSword.h"
#include "Charactor/NormalSpear.h"
#include "Charactor/StrongSpear.h"

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

    //------< 定数 >------------------------------------------------------------

    static const int CHIP_NUM_X = 225;       // マップの横方向のチップ数
    static const int CHIP_NUM_Y = 926;       // マップの縦方向のチップ数
    static const int CHIP_SIZE = 3;       // %演算子を使用するためint型を使用する
    static const int NORMAL_ENEMY_NUM = 170;       // normal_enemyの数
    static const int BOMB_ENEMY_NUM = 108;       // normal_enemyの数

private:
    //------< 変数 >------------------------------------------------------------

    Block Arrangement[CHIP_NUM_Y][CHIP_NUM_X];                   // 敵の配置データ
    /*int normal_enemy_limit_num = 30;
    int bomb_enemy_limit_num = 20;
    int normal_enemy_num[3] = {0};
    int bomb_enemy_num[3] = {0};
    int random_num = 0; 

    float random_scale = 1;
    int random_angle = 0;

    int normal_random = 0;
    bool bomb_random = false;*/


public:
    Enemy_Arrangement() {};
    ~Enemy_Arrangement() {};


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