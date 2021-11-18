
#include "audioManager.h"
#include "common.h"

void AudioManager::Init()
{
    //bgm_normal       = Audio::Instance().LoadAudioSource("Data/Audio/BGM/通常.wav");
    //bgm_speed        = Audio::Instance().LoadAudioSource("Data/Audio/BGM/ラストスパート.wav");
    //se_select        = Audio::Instance().LoadAudioSource("Data/Audio/SE/選択.wav");
    //se_enter         = Audio::Instance().LoadAudioSource("Data/Audio/SE/コマンド.wav");
    //se_success       = Audio::Instance().LoadAudioSource("Data/Audio/SE/チュートリアル成功.wav");
    //se_wallhit       = Audio::Instance().LoadAudioSource("Data/Audio/SE/ぶつかり.wav");
    //se_growup        = Audio::Instance().LoadAudioSource("Data/Audio/SE/成長.wav");
    //se_timeup        = Audio::Instance().LoadAudioSource("Data/Audio/SE/終了音.wav");
    //se_result        = Audio::Instance().LoadAudioSource("Data/Audio/SE/リザルト.wav");


    //追加
    bgm_title = Audio::Instance().LoadAudioSource("Data/Audio/Result/GameTitle.wav");
    bgm_gameclear = Audio::Instance().LoadAudioSource("Data/Audio/Result/GameClear.wav");
    bgm_gameover = Audio::Instance().LoadAudioSource("Data/Audio/Result/GameOver.wav");

    bgm_stage1 = Audio::Instance().LoadAudioSource("Data/Audio/StageBGM/Stage1.wav");
    //bgm_stage2 = Audio::Instance().LoadAudioSource("Data/Audio/StageBGM/Stage2.wav");
    bgm_stage2 = Audio::Instance().LoadAudioSource("Data/Audio/StageBGM/Stage2_2.wav");
    //bgm_stage3 = Audio::Instance().LoadAudioSource("Data/Audio/StageBGM/Stage3.wav");
    bgm_stage3 = Audio::Instance().LoadAudioSource("Data/Audio/StageBGM/Stage3_2.wav");
    bgm_stage4 = Audio::Instance().LoadAudioSource("Data/Audio/StageBGM/Stage4.wav");

    se_click = Audio::Instance().LoadAudioSource("Data/Audio/Result/Click.wav");

    se_player_attack1 = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Player/Attack1.wav");
    se_player_attack2 = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Player/Attack2.wav");
    se_player_attack3 = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Player/Attack3.wav");
    se_player_damage = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Player/Damage.wav");

    se_enemy_run = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Enemy/Run.wav");
    se_enemy_spear_attack = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Enemy/SpearAttack.wav");
    se_enemy_sword_attack = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Enemy/SwordAttack.wav");
    se_enemy_walk = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Enemy/Walk.wav");
    se_enemy_die = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Enemy/Die.wav");
    se_enemy_damage = Audio::Instance().LoadAudioSource("Data/Audio/CharacterSE/Enemy/Damage.wav");
}


void AudioManager::Deinit()
{
    //bgm_normal      .reset();
    //bgm_speed       .reset();
    //se_select       .reset();
    //se_enter        .reset();
    //se_success      .reset();
    //se_wallhit      .reset();
    //se_growup       .reset();
    //se_timeup       .reset();
    //se_result       .reset();
    
    //追加
    bgm_title                   .reset();
    bgm_gameover                .reset();
    bgm_gameclear               .reset();

    bgm_stage1                  .reset();
    bgm_stage2                  .reset();
    bgm_stage3                  .reset();
    bgm_stage4                  .reset();

    se_click.reset();

    se_player_attack1           .reset();
    se_player_attack2           .reset();
    se_player_attack3           .reset();
    se_player_damage           .reset();

    se_enemy_run                .reset();
    se_enemy_spear_attack       .reset();
    se_enemy_sword_attack       .reset();
    se_enemy_walk               .reset();
    se_enemy_die               .reset();
    se_enemy_damage.reset();


}