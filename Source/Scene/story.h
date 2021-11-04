#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "C_function.h"

//ゲーム開始前のストーリー説明のシーン

class Story : public Scene
{
public:
    Story() {}
    ~Story()override {}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

private:
    std::unique_ptr<Sprite> spr_haikei = nullptr;
    std::unique_ptr<Sprite> spr_start = nullptr;
    std::unique_ptr<Sprite> spr_frame = nullptr;
    std::unique_ptr<Sprite> spr_tutorial = nullptr;
    std::unique_ptr<Sprite> spr_mouseCursor = nullptr;
    Pos mousePos;

    //当たり判定
    C_Hitbox MouseBox;
    C_Hitbox StartBox;
    C_Hitbox tutorialBox;
    bool hit = false;
    bool start_check = false;
    bool tutorial_check = false;
    int check_state = 0;

    Pos startPos;
    Pos tutorialPos;
    Pos framePos;

};
