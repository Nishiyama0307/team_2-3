#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "C_function.h"


// タイトルシーン

class Title : public Scene
{
public:
    Title(){}
    ~Title()override{}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

private:
    std::unique_ptr<Sprite> spr_title_logo = nullptr;

    int selecting = 0;
    int select_timer = 0;
    bool game_mode_select = false;

    float angle = 0.0f;

    enum Select
    {
        START,
        END,
        TUTORIAL,
        GAME,
    };

    std::unique_ptr<Sprite> spr_mouseCursor = nullptr;
    Pos mousePos;

    std::unique_ptr<Sprite> spr_start = nullptr;
    std::unique_ptr<Sprite> spr_endbox = nullptr;

    std::unique_ptr<Sprite> spr_wood1 = nullptr;
    std::unique_ptr<Sprite> spr_wood2 = nullptr;

    float rgb_A1 = 0.4f;
    float rgb_A2 = 0.4f;

    //当たり判定
    C_Hitbox MouseBox;
    C_Hitbox StartBox;
    C_Hitbox EndBox;
    bool hit = false;
    bool start_check = false;
    bool end_check = false;
    int check_state = 0;

    Pos startPos;
    Pos startPos2;
    Pos endPos;
    Pos endPos2;
    Pos framePos;

    //点滅
    int timer1 = 0;
    int timer2 = 0;

};