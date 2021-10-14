#pragma once

#include "Graphics/Graphics.h"
#include "Input\Input.h"
class SceneManager;



// �V�[���N���X(���ۃN���X)

class Scene
{
public:
    Scene() {};
    virtual ~Scene() {};

    virtual void Update(float elapsedTime) = 0;
    virtual void ModelRender(ID3D11DeviceContext* dc, Shader* shader) = 0;
    virtual void SpriteRender(ID3D11DeviceContext* dc) = 0;
    virtual void DebugRender() {}

    // ImGui�̕\��
    virtual void ImGui() {};

    // �V�[���̏I������
    virtual void DeInit() = 0;

    // �V�[���̏�������
    void Init(SceneManager* sceneManager);

    // �ݒ�
    virtual void Set() = 0;

    // ���f���E�e�N�X�`���ǂݍ���
    virtual void Load() = 0;


public: // Get�֐�
    bool IsSceneLoaded() { return scene_loaded; }

public: // Set�֐�
    void SetSceneLoaded() { scene_loaded = true; }
    void ChangeNextScene(Scene* next_scene_, GamePadButton buttom, bool isloading = true);
    void ChangeNextScene(Scene* next_scene_, bool isloading = true);

public:
    DirectX::XMFLOAT4 back_color = { 1.0f,1.0f,1.0f,1.0f }; // �w�i�F

protected:
    SceneManager* GetSceneManager() { return scene_manager; }

private:
    SceneManager* scene_manager = nullptr;
    bool scene_loaded = false;  // ���[�h���ꂽ��

public:
    // BGM�p
    bool bgm_normal = false;
    bool bgm_caution = false;
};