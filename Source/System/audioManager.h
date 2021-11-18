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
    SE_GROWUP,
    SE_TIMEUP,
    SE_RESULT,


    //追加
    BGM_TITLE,
    BGM_GAMECLEAR,
    BGM_GAMEOVER,

    BGM_STAGE1,
    BGM_STAGE2,
    BGM_STAGE3,
    BGM_STAGE4,

    SE_CLICK,

    SE_PLAYER_ATTACK1,
    SE_PLAYER_ATTACK2,
    SE_PLAYER_ATTACK3,
    SE_PLAYER_DAMAGE,

    SE_ENEMY_RUN,
    SE_ENEMY_SPEAR_ATTACK,
    SE_ENEMY_SWORD_ATTACK,
    SE_ENEMY_WALK,
    SE_ENEMY_DIE,
    SE_ENEMY_DAMAGE,
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
        //if(index == Audio_INDEX::BGM_NORMAL)        return bgm_normal      .get();
        //if(index == Audio_INDEX::BGM_SPEED)         return bgm_speed       .get();
        //if(index == Audio_INDEX::SE_SELECT)         return se_select       .get();
        //if(index == Audio_INDEX::SE_ENTER)          return se_enter        .get();
        //if(index == Audio_INDEX::SE_SUCCESS)        return se_success      .get();
        //if(index == Audio_INDEX::SE_WALLHIT)        return se_wallhit      .get();
        //if(index == Audio_INDEX::SE_GROWUP)         return se_growup       .get();
        //if(index == Audio_INDEX::SE_TIMEUP)         return se_timeup       .get();
        //if(index == Audio_INDEX::SE_RESULT)         return se_result       .get();

        //追加
        if (index == Audio_INDEX::BGM_TITLE)                    return bgm_title.get();
        if (index == Audio_INDEX::BGM_GAMECLEAR)                return bgm_gameclear.get();
        if (index == Audio_INDEX::BGM_GAMEOVER)                 return bgm_gameover.get();

        if (index == Audio_INDEX::BGM_STAGE1)                   return bgm_stage1.get();
        if (index == Audio_INDEX::BGM_STAGE2)                   return bgm_stage2.get();
        if (index == Audio_INDEX::BGM_STAGE3)                   return bgm_stage3.get();
        if (index == Audio_INDEX::BGM_STAGE4)                   return bgm_stage4.get();

        if (index == Audio_INDEX::SE_CLICK)                     return se_click.get();

        if (index == Audio_INDEX::SE_PLAYER_ATTACK1)            return se_player_attack1.get();
        if (index == Audio_INDEX::SE_PLAYER_ATTACK2)            return se_player_attack2.get();
        if (index == Audio_INDEX::SE_PLAYER_ATTACK3)            return se_player_attack3.get();
        if (index == Audio_INDEX::SE_PLAYER_DAMAGE)            return se_player_damage.get();

        if (index == Audio_INDEX::SE_ENEMY_RUN)                 return se_enemy_run.get();
        if (index == Audio_INDEX::SE_ENEMY_SPEAR_ATTACK)        return se_enemy_spear_attack.get();
        if (index == Audio_INDEX::SE_ENEMY_SWORD_ATTACK)        return se_enemy_sword_attack.get();
        if (index == Audio_INDEX::SE_ENEMY_WALK)                return se_enemy_walk.get();
        if (index == Audio_INDEX::SE_ENEMY_DIE)                return se_enemy_die.get();
        if (index == Audio_INDEX::SE_ENEMY_DAMAGE)                return se_enemy_damage.get();
    }

public: // Set関数
    
private:
    //std::unique_ptr<AudioSource> bgm_normal             = nullptr;
    //std::unique_ptr<AudioSource> bgm_speed              = nullptr;
    //std::unique_ptr<AudioSource> se_select              = nullptr;
    //std::unique_ptr<AudioSource> se_enter               = nullptr;
    //std::unique_ptr<AudioSource> se_success             = nullptr;
    //std::unique_ptr<AudioSource> se_wallhit             = nullptr;
    //std::unique_ptr<AudioSource> se_growup              = nullptr;
    //std::unique_ptr<AudioSource> se_timeup              = nullptr;
    //std::unique_ptr<AudioSource> se_result              = nullptr;

    //追加
    std::unique_ptr<AudioSource> bgm_title              = nullptr;
    std::unique_ptr<AudioSource> bgm_gameclear          = nullptr;
    std::unique_ptr<AudioSource> bgm_gameover           = nullptr;

    std::unique_ptr<AudioSource> bgm_stage1             = nullptr;
    std::unique_ptr<AudioSource> bgm_stage2             = nullptr;
    std::unique_ptr<AudioSource> bgm_stage3             = nullptr;
    std::unique_ptr<AudioSource> bgm_stage4             = nullptr;

    std::unique_ptr<AudioSource> se_click             = nullptr;

    std::unique_ptr<AudioSource> se_player_attack1      = nullptr;
    std::unique_ptr<AudioSource> se_player_attack2      = nullptr;
    std::unique_ptr<AudioSource> se_player_attack3      = nullptr;
    std::unique_ptr<AudioSource> se_player_damage      = nullptr;

    std::unique_ptr<AudioSource> se_enemy_run               = nullptr;
    std::unique_ptr<AudioSource> se_enemy_spear_attack      = nullptr;
    std::unique_ptr<AudioSource> se_enemy_sword_attack      = nullptr;
    std::unique_ptr<AudioSource> se_enemy_walk              = nullptr;
    std::unique_ptr<AudioSource> se_enemy_die              = nullptr;
    std::unique_ptr<AudioSource> se_enemy_damage              = nullptr;

};