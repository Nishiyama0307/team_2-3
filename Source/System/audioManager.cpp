
#include "audioManager.h"
#include "common.h"

void AudioManager::Init()
{
    bgm_normal       = Audio::Instance().LoadAudioSource("Data/Audio/BGM/�ʏ�.wav");
    bgm_speed        = Audio::Instance().LoadAudioSource("Data/Audio/BGM/���X�g�X�p�[�g.wav");
    se_select        = Audio::Instance().LoadAudioSource("Data/Audio/SE/�I��.wav");
    se_enter         = Audio::Instance().LoadAudioSource("Data/Audio/SE/�R�}���h.wav");
    se_success       = Audio::Instance().LoadAudioSource("Data/Audio/SE/�`���[�g���A������.wav");
    se_wallhit       = Audio::Instance().LoadAudioSource("Data/Audio/SE/�Ԃ���.wav");
    se_inhale_normal = Audio::Instance().LoadAudioSource("Data/Audio/SE/�z�����݁i�����L�j.wav");
    se_inhale_bomb   = Audio::Instance().LoadAudioSource("Data/Audio/SE/�z�����݁i�����L�j.wav");
    se_growup        = Audio::Instance().LoadAudioSource("Data/Audio/SE/����.wav");
    se_timeup        = Audio::Instance().LoadAudioSource("Data/Audio/SE/�I����.wav");
    se_result        = Audio::Instance().LoadAudioSource("Data/Audio/SE/���U���g.wav");
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