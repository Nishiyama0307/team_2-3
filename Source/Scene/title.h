#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"



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
    std::unique_ptr<Sprite> spr_space = nullptr;
    std::unique_ptr<Sprite> spr_play = nullptr;
    std::unique_ptr<Sprite> spr_end = nullptr;
    std::unique_ptr<Sprite> spr_Tutorial_moji = nullptr;
    std::unique_ptr<Sprite> spr_Game_moji = nullptr;

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
    
};