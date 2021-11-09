#include "Charactor/Enemy_Arrangement.h"
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

        //delim����؂蕶���Ƃ��Đ؂蕪���Aint�ɕϊ�����data[][]�Ɋi�[����
        for (string::size_type spos, epos = 0; (spos = line.find_first_not_of(delim, epos)) != string::npos;)
        {
            string token = line.substr(spos, (epos = line.find_first_of(delim, spos)) - spos);
            data[row][col++].num = stoi(token);
        }

        ++row;
    }
}

//// �X�V
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
    // �G�l�~�[������
    EnemyManager& enemyManager = EnemyManager::Instance();
    
    switch (csv_file)
    {
    case TUTORIAL_NORMAL:
        csv_load(Arrangement,"Data/map/enemy_arrangement._�`���[�g���A��(normal).csv");
        break;
    case TUTORIAL_BOMB:
        csv_load(Arrangement,"Data/map/enemy_arrangement._�`���[�g���A��(bomb).csv");
        break;
    case GAME:
        csv_load(Arrangement,"Data/map/�z�u�}_test_�c�̍s��.csv");
        break;
    default:
        break;
    }

    for (int y = 0; y < CHIP_NUM_Y; y++)
    {
        for (int x = 0; x < CHIP_NUM_X; x++)
        {
            // ��_�␳�@�X�e�[�W�̍���̍��W����n�܂�悤��
            constexpr float Reference_point_correction = -499;
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
            if (Arrangement[y][x].num == ENEMYTAG::NORMAL_SWORD)
            {

                NormalSword* normalSword = new NormalSword();
                normalSword->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                normalSword->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
                enemyManager.Register(normalSword, ENEMYTAG::NORMAL_SWORD);
            }
            if (Arrangement[y][x].num == ENEMYTAG::STRONG_SWORD)
            {

                StrongSword* strongSword = new StrongSword();
                strongSword->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                strongSword->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
                enemyManager.Register(strongSword, ENEMYTAG::STRONG_SWORD);
            }
            if (Arrangement[y][x].num == ENEMYTAG::NORMAL_SPEAR)
            {

                NormalSpear* normalSpear = new NormalSpear();
                normalSpear->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                normalSpear->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
                enemyManager.Register(normalSpear, ENEMYTAG::NORMAL_SPEAR);
            }
            if (Arrangement[y][x].num == ENEMYTAG::STRONG_SPEAR)
            {

                StrongSpear* strongSpear = new StrongSpear();
                strongSpear->SetPosition(DirectX::XMFLOAT3(Reference_point_correction + x * CHIP_SIZE, 0, Reference_point_correction + y * CHIP_SIZE));
                strongSpear->SetAngle({ 0, DirectX::XMConvertToRadians(180), 0 });
                enemyManager.Register(strongSpear, ENEMYTAG::STRONG_SPEAR);
            }
        }
    }
}