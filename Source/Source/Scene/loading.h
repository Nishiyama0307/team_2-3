#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "sceneManager.h"



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
    Sprite* spr_icon = nullptr;
    std::unique_ptr<Sprite> spr_back = nullptr;
    float angle = 0.0f;

};