
#include "audioManager.h"
#include "common.h"

void AudioManager::Init()
{
    bgm_normal       = Audio::Instance().LoadAudioSource("Data/Audio/BGM/通常.wav");
    bgm_speed        = Audio::Instance().LoadAudioSource("Data/Audio/BGM/ラストスパート.wav");
    se_select        = Audio::Instance().LoadAudioSource("Data/Audio/SE/選択.wav");
    se_enter         = Audio::Instance().LoadAudioSource("Data/Audio/SE/コマンド.wav");
    se_success       = Audio::Instance().LoadAudioSource("Data/Audio/SE/チュートリアル成功.wav");
    se_wallhit       = Audio::Instance().LoadAudioSource("Data/Audio/SE/ぶつかり.wav");
    se_inhale_normal = Audio::Instance().LoadAudioSource("Data/Audio/SE/吸い込み（いい貝）.wav");
    se_inhale_bomb   = Audio::Instance().LoadAudioSource("Data/Audio/SE/吸い込み（悪い貝）.wav");
    se_growup        = Audio::Instance().LoadAudioSource("Data/Audio/SE/成長.wav");
    se_timeup        = Audio::Instance().LoadAudioSource("Data/Audio/SE/終了音.wav");
    se_result        = Audio::Instance().LoadAudioSource("Data/Audio/SE/リザルト.wav");
}


void AudioManager::Deinit()
{
    bgm_normal      .reset();
    bgm_speed       .reset();
    se_select       .reset();
    se_enter        .reset();
    se_success      .reset();
    se_wallhit      .reset();
    se_inhale_normal.reset();
    se_inhale_bomb  .reset();
    se_growup       .reset();
    se_timeup       .reset();
    se_result       .reset();
}