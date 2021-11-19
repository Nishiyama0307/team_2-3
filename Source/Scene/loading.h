#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "sceneManager.h"

#include "C_function.h"

// åãâ ÉVÅ[Éì

class Loading : public Scene
{
public:
    Loading(Scene* load_scene) : scene(load_scene) {}
    ~Loading()override {}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void ImGui()override;

    void DeInit()override;

    void Set()override;
    void Load()override;

private:
    static void LoadingThread(Loading* load, SceneManager* scene_manager_);

private:
    Scene* scene = nullptr;
    float angle = 0.0f;

    std::unique_ptr<Sprite> spr_load = nullptr;
    std::unique_ptr<Sprite> spr_load_word = nullptr;
    Pos  texPos;
    float LoadTimer;
};