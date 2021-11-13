#pragma once


#include "Audio\Audio.h"
#include <map>


enum class Audio_INDEX
{
    BGM_NORMAL,
    BGM_SPEED,
    SE_SELECT,
    SE_ENTER,
    SE_SUCCESS,
    SE_WALLHIT,
    SE_INHALE_NORMAL,
    SE_INHALE_BOMB,
    SE_GROWUP,
    SE_TIMEUP,
    SE_RESULT,
};


// オーディオマネージャー

class AudioManager
{
private:
    AudioManager() {};
    ~AudioManager() {};

public:
    static AudioManager& Instance()
    {
        static AudioManager instance;
        return instance;
    }
    void Init();
    void Deinit();

public: // Get関数
    AudioSource* GetAudio(Audio_INDEX index)
    {
        if(index == Audio_INDEX::BGM_NORMAL)        return bgm_normal      .get();
        if(index == Audio_INDEX::BGM_SPEED)         return bgm_speed       .get();
        if(index == Audio_INDEX::SE_SELECT)         return se_select       .get();
        if(index == Audio_INDEX::SE_ENTER)          return se_enter        .get();
        if(index == Audio_INDEX::SE_SUCCESS)        return se_success      .get();
        if(index == Audio_INDEX::SE_WALLHIT)        return se_wallhit      .get();
        if(index == Audio_INDEX::SE_INHALE_NORMAL)  return se_inhale_normal.get();
        if(index == Audio_INDEX::SE_INHALE_BOMB)    return se_inhale_bomb  .get();
        if(index == Audio_INDEX::SE_GROWUP)         return se_growup       .get();
        if(index == Audio_INDEX::SE_TIMEUP)         return se_timeup       .get();
        if(index == Audio_INDEX::SE_RESULT)         return se_result       .get();
    }

public: // Set関数
    
private:
    std::unique_ptr<AudioSource> bgm_normal             = nullptr;
    std::unique_ptr<AudioSource> bgm_speed              = nullptr;
    std::unique_ptr<AudioSource> se_select              = nullptr;
    std::unique_ptr<AudioSource> se_enter               = nullptr;
    std::unique_ptr<AudioSource> se_success             = nullptr;
    std::unique_ptr<AudioSource> se_wallhit             = nullptr;
    std::unique_ptr<AudioSource> se_inhale_normal       = nullptr;
    std::unique_ptr<AudioSource> se_inhale_bomb         = nullptr;
    std::unique_ptr<AudioSource> se_growup              = nullptr;
    std::unique_ptr<AudioSource> se_timeup              = nullptr;
    std::unique_ptr<AudioSource> se_result              = nullptr;

};