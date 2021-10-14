#pragma once

#include "common.h"
#include "Audio/Audio.h"
class Scene;



// シーン遷移を管理するオブジェクト

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Update(float elapsedTime);
    void Render(float elapsedTime);

    // シーン解放
    void DeInit();

public: // Get関数

public: // Set関数
    void SetLoadNextScene(Scene* next_scene_);  // ロード込みのシーン遷移
    void SetNextScene(Scene* next_scene_);      // ロードなしのシーン遷移
    
private:
    // シーン変更
    void Init();

    // ImGui描画
    void ImGui();

    void ChangeScene();

private:
    Scene* now_scene = nullptr;
    Scene* next_scene = nullptr;
};