#pragma once

#include "common.h"
#include "Audio/Audio.h"
class Scene;



// �V�[���J�ڂ��Ǘ�����I�u�W�F�N�g

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void Update(float elapsedTime);
    void Render(float elapsedTime);

    // �V�[�����
    void DeInit();

public: // Get�֐�

public: // Set�֐�
    void SetLoadNextScene(Scene* next_scene_);  // ���[�h���݂̃V�[���J��
    void SetNextScene(Scene* next_scene_);      // ���[�h�Ȃ��̃V�[���J��
    
private:
    // �V�[���ύX
    void Init();

    // ImGui�`��
    void ImGui();

    void ChangeScene();

private:
    Scene* now_scene = nullptr;
    Scene* next_scene = nullptr;
};