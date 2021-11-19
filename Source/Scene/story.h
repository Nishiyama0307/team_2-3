#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "C_function.h"

//�Q�[���J�n�O�̃X�g�[���[�����̃V�[��

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
    std::unique_ptr<Sprite> spr_tutorial = nullptr;
    std::unique_ptr<Sprite> spr_mouseCursor = nullptr;
    std::unique_ptr<Sprite> spr_question = nullptr;

    Pos mousePos;

    //�X�g�[���[��1���G
    int story_state = 0;
    std::unique_ptr<Sprite> spr_story1 = nullptr;
    std::unique_ptr<Sprite> spr_story2 = nullptr;
    std::unique_ptr<Sprite> spr_story3 = nullptr;
    std::unique_ptr<Sprite> spr_story4 = nullptr;
    std::unique_ptr<Sprite> spr_story5 = nullptr;
    std::unique_ptr<Sprite> spr_story6 = nullptr;

    float posx1;
    float posx2;
    float posx3;
    float posx4;
    float posx5;
    float posx6;
    bool is_change = false;

    //�I��
    std::unique_ptr<Sprite> spr_wood1 = nullptr;
    std::unique_ptr<Sprite> spr_wood2 = nullptr;
    std::unique_ptr<Sprite> spr_wood3 = nullptr;

    //�����蔻��
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

    bool check_start = false;

    //�_��
    int timer1 = 0;
    int timer2 = 0;

};
