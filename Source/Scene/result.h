#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"



// åãâ ÉVÅ[Éì

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



    // çïë—
    std::unique_ptr<Sprite> black_band = nullptr;
    const float black_band_timer_max = 1.4f;
    float black_band_timer = 0.0f;
    bool did_first = false;
    bool did = false;

};