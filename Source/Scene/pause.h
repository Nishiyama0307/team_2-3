#pragma once

#include "Graphics/Sprite.h"
#include <memory>
#include "scene.h"

#include "C_function.h"

// ポーズクラス

class Pause
{
public:
    Pause(Scene* scene_);
    ~Pause();

public:
    bool Update(float elapsedTime);
    void SpriteRender(ID3D11DeviceContext* dc);
    
public: // Get関数



private:
    Scene* scene = nullptr;
    bool now_pause = false;
    std::unique_ptr<Sprite> spr_back = nullptr;
    std::unique_ptr<Sprite> spr_play = nullptr;
    std::unique_ptr<Sprite> spr_end = nullptr;
    std::unique_ptr<Sprite> spr_ui = nullptr;

    int selecting = 0;
    int select_timer = 0;
    float angle = 0.0f;

    enum Select
    {
        CONTINUE,
        END,
    };

    //ここ
    std::unique_ptr<Sprite> spr_mouseCursor = nullptr;
    Pos mousePos;

    std::unique_ptr<Sprite> spr_start = nullptr;
    std::unique_ptr<Sprite> spr_frame = nullptr;
    std::unique_ptr<Sprite> spr_endbox = nullptr;
    std::unique_ptr<Sprite> spr_back2 = nullptr;

    std::unique_ptr<Sprite> spr_wood1 = nullptr;
    std::unique_ptr<Sprite> spr_wood2 = nullptr;

    //当たり判定
    C_Hitbox MouseBox;
    C_Hitbox StartBox;
    C_Hitbox EndBox;
    bool hit = false;
    bool start_check = false;
    bool end_check = false;
    int check_state = 0;

    Pos startPos;
    Pos endPos;
    Pos framePos;

    //点滅
    int timer1 = 0;
    int timer2 = 0;
};