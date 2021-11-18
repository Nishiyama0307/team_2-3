#include "Charactor/Enemy_Arrangement.h"

#include "Scene/game.h"
//#include "Enemy.h"

void csv_load(Block data[][Enemy_Arrangement::CHIP_NUM_X], const char* filename)
{
    ifstream stream(filename);
    string line;
    //int data[45][15];
    const string delim = ",";

    int row = 0;
    int col;
    while (getline(stream, line))
    {
        col = 0;

        //delimを区切り文字として切り分け、intに変換してdata[][]に格納する
        for (string::size_type spos, epos = 0; (spos = line.find_first_not_of(delim, epos)) != string::npos;)
        {
            string token = line.substr(spos, (epos = line.find_first_of(delim, spos)) - spos);
            data[row][col++].num = stoi(token);
        }

        ++row;
    }
}

//// 更新
//void Enemy_Arrangement::update()
//{
//
//}

//void Enemy_Arrangement::clear()
//{
//
//}

void Enemy_Arrangement::enemy_produce(csv_file_num csv_file)
{
    // エネミー初期化
    EnemyManager& enemyManager = EnemyManager::Instance();
    
    switch (csv_file)
    {
    case TUTORIAL_NORMAL:
        csv_load(Arrangement,"Data/map/enemy_arrangement._チュートリアル(normal).csv");
        break;
    case TUTORIAL_BOMB:
        csv_load(Arrangement,"Data/map/enemy_arrangement._チュートリアル(bomb).csv");
        break;
    case GAME:
#ifdef _DEBUG
        //csv_load(Arrangement, "Data/map/配置図(size更新)_test_分割stage1.csv");
        //csv_load(Arrangement, "Data/map/配置図(size更新)_test_分割.csv");
        csv_load(Arrangement, "Data/map/配置図(size更新)_本番.csv");
#else
        csv_load(Arrangement, "Data/map/配置図(size更新)_本番.csv");
#endif
        break;
    default:
        break;
    }

	constexpr int kMap_Size_Hosei{ -175 };

   for (int y = 0; y < CHIP_NUM_Y; y++)
   {
       float step_offset{ 0.0f };

       //if (kStage2_Start_Position < (y + kMap_Size_Hosei) * CHIP_SIZE && (y + kMap_Size_Hosei) * CHIP_SIZE < kStage3_Start_Position) step_offset = 2.0f;
       if (kStage2_Start_Position < y  * CHIP_SIZE + kMap_Size_Hosei && y  * CHIP_SIZE + kMap_Size_Hosei < kStage3_Start_Position) step_offset = 7.0f;

       for (int x = 0; x < CHIP_NUM_X; x++)
       {
           // 基準点補正　ステージの左上の座標から始まるように
           //constexpr float Reference_point_correction = -499;
           constexpr DirectX::XMFLOAT2 Reference_point_correction = { -337, -347 + 175 }; // (CHIP_NUM / 2) x: -337, y: -347
       	//float randam_scale = 1;
           constexpr float Fixed_value_angle = 22.5f;

           //if (Arrangement[y][x].num == ENEMYTAG::NORMAL)
           //{
           //    if ((y < 100 || y > 150) /*&& (x < 100 || x > 150)*/)
           //        if (csv_file == csv_file_num::GAME) random_scaling(Arrangement[y][x].num);

           //    NormalEnemy* normalEnemy = new NormalEnemy();
           //    normalEnemy->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));

           //    normalEnemy->SetScale({ random_scale, random_scale, random_scale });
           //    if (csv_file == csv_file_num::TUTORIAL_NORMAL) normalEnemy->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
           //    if (csv_file == csv_file_num::GAME)
           //    {
           //        random_angling();
           //        normalEnemy->SetAngle({ 0, DirectX::XMConvertToRadians(Fixed_value_angle * random_angle), 0 });
           //    }
           //    enemyManager.Register(normalEnemy, ENEMYTAG::NORMAL);
           //}
           //if (Arrangement[y][x].num == ENEMYTAG::BOMB)
           //{       
           //    if ((y < 100 || y > 150) /*&& (x < 100 || x > 150)*/)
           //        if (csv_file == csv_file_num::GAME) random_scaling(Arrangement[y][x].num);
           //    BombEnemy* bombEnemy = new BombEnemy();
           //    bombEnemy->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
           //    
           //    bombEnemy->SetScale({ random_scale, random_scale, random_scale });
           //    if (csv_file == csv_file_num::TUTORIAL_BOMB) bombEnemy->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
           //    if (csv_file == csv_file_num::GAME)
           //    {
           //        random_angling();
           //        bombEnemy->SetAngle({ 0, DirectX::XMConvertToRadians(Fixed_value_angle * random_angle), 0 });
           //    }
           //    enemyManager.Register(bombEnemy, ENEMYTAG::BOMB);
           //}
           

           int stage_num;
           if (kStage2_Start_Position > (y ) * CHIP_SIZE + kMap_Size_Hosei)
           {
               stage_num = 0;
           }
           else if (kStage3_Start_Position > (y ) * CHIP_SIZE + kMap_Size_Hosei)
           {
               stage_num = 1;
           }
           else if (kStage4_Start_Position > (y ) * CHIP_SIZE + kMap_Size_Hosei)
           {
               stage_num = 2;
           }
           else
           {
               stage_num = 3;
           }
           /*int stage_num;
           if (kStage2_Start_Position > y  * CHIP_SIZE)
           {
               stage_num = 0;
           }
           else if (kStage3_Start_Position > y  * CHIP_SIZE)
           {
               stage_num = 1;
           }
           else if (kStage4_Start_Position > y  * CHIP_SIZE)
           {
               stage_num = 2;
           }
           else
           {
               stage_num = 3;
           }*/

           if (Arrangement[y][x].num == ENEMYTAG::NORMAL_SWORD)
           {

               NormalSword* normalSword = new NormalSword();
               //normalSword->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + (y + kMap_Size_Hosei) * CHIP_SIZE));
               normalSword->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + y * CHIP_SIZE));
               normalSword->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
               normalSword->SetStageNum(stage_num);
               enemyManager.Register(normalSword, ENEMYTAG::NORMAL_SWORD);
           }
           if (Arrangement[y][x].num == ENEMYTAG::STRONG_SWORD)
           {

               StrongSword* strongSword = new StrongSword();
               //strongSword->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + (y + kMap_Size_Hosei) *CHIP_SIZE));
               strongSword->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + y * CHIP_SIZE));
               strongSword->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
               strongSword->SetStageNum(stage_num);
               enemyManager.Register(strongSword, ENEMYTAG::STRONG_SWORD);
           }
           if (Arrangement[y][x].num == ENEMYTAG::NORMAL_SPEAR)
           {

               NormalSpear* normalSpear = new NormalSpear();
               //normalSpear->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + (y + kMap_Size_Hosei) *CHIP_SIZE));
               normalSpear->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + y * CHIP_SIZE));
               normalSpear->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
               normalSpear->SetStageNum(stage_num);
               enemyManager.Register(normalSpear, ENEMYTAG::NORMAL_SPEAR);
           }
           if (Arrangement[y][x].num == ENEMYTAG::STRONG_SPEAR)
           {

               StrongSpear* strongSpear = new StrongSpear();
               //strongSpear->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + (y + kMap_Size_Hosei) *CHIP_SIZE));
               strongSpear->SetPosition(DirectX::XMFLOAT3(Reference_point_correction.x + x * CHIP_SIZE, step_offset, Reference_point_correction.y + y * CHIP_SIZE));
               strongSpear->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
               strongSpear->SetStageNum(stage_num);
               enemyManager.Register(strongSpear, ENEMYTAG::STRONG_SPEAR);
           }
       }
   }
}