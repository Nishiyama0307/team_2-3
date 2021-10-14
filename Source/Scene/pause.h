#pragma once

#include "Graphics/Sprite.h"
#include <memory>
#include "scene.h"



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
};