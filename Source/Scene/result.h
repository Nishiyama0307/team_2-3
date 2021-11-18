#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "C_function.h"


// 結果シーン

class Result : public Scene
{
public:
    Result() {}
    ~Result()override {}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

private:
    void ChangeScene(float elapsedTime);
    void ChangeSceneSpriteRender(ID3D11DeviceContext* dc);

private:

    std::unique_ptr<Sprite> spr_back = nullptr;
    std::unique_ptr<Sprite> spr_retry = nullptr;
    std::unique_ptr<Sprite> spr_end = nullptr;
    std::unique_ptr<Sprite> spr_font = nullptr;

    int selecting = 0;
    int select_timer = 0;

    float angle = 0.0f;

    enum Select
    {
        RETRY,
        END,
    };



    // 黒帯
    std::unique_ptr<Sprite> black_band = nullptr;
    const float black_band_timer_max = 1.4f;
    float black_band_timer = 0.0f;
    bool did_first = false;
    bool did = false;


    //クリア、ゲームオーバー１、ゲームオーバー２
    std::unique_ptr<Sprite> spr_clear = nullptr;
    std::unique_ptr<Sprite> spr_over1 = nullptr;
    std::unique_ptr<Sprite> spr_over2 = nullptr;
    std::unique_ptr<Sprite> spr_over3 = nullptr;

    std::unique_ptr<Sprite> spr_mouseCursor = nullptr;
    Pos mousePos;
    C_Hitbox MouseBox;

    std::unique_ptr<Sprite> spr_wood1 = nullptr;
    std::unique_ptr<Sprite> spr_back2 = nullptr;
    Pos backPos;
    C_Hitbox backBox;

    bool hit;
    bool back_check = false;
    int timer1 = 0;

};

enum 
{
    Game_clear,
    Game_over1,
    Game_over2,
    Game_over3
};