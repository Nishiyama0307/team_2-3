#pragma once

#include "Graphics/Graphics.h"
#include "Input\Input.h"
class SceneManager;



// シーンクラス(抽象クラス)

class Scene
{
public:
    Scene() {};
    virtual ~Scene() {};

    virtual void Update(float elapsedTime) = 0;
    virtual void ModelRender(ID3D11DeviceContext* dc, Shader* shader) = 0;
    virtual void SpriteRender(ID3D11DeviceContext* dc) = 0;
    virtual void DebugRender() {}

    // ImGuiの表示
    virtual void ImGui() {};

    // シーンの終了処理
    virtual void DeInit() = 0;

    // シーンの初期処理
    void Init(SceneManager* sceneManager);

    // 設定
    virtual void Set() = 0;

    // モデル・テクスチャ読み込み
    virtual void Load() = 0;


public: // Get関数
    bool IsSceneLoaded() { return scene_loaded; }

public: // Set関数
    void SetSceneLoaded() { scene_loaded = true; }
    void ChangeNextScene(Scene* next_scene_, GamePadButton buttom, bool isloading = true);
    void ChangeNextScene(Scene* next_scene_, bool isloading = true);

public:
    DirectX::XMFLOAT4 back_color = { 1.0f,1.0f,1.0f,1.0f }; // 背景色

protected:
    SceneManager* GetSceneManager() { return scene_manager; }

private:
    SceneManager* scene_manager = nullptr;
    bool scene_loaded = false;  // ロードされたか

public:
    // BGM用
    bool bgm_normal = false;
    bool bgm_caution = false;
};