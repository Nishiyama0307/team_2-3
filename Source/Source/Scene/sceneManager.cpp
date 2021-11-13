
#include "Graphics/Graphics.h"
#include "Camera.h"
#include "sceneManager.h"
#include "scene.h"
#include "title.h"
#include "loading.h"

#include "EffectManager.h"
#include "gameSystem.h"



SceneManager::SceneManager()
{
    GameSystem::Instance().Init();
}


SceneManager::~SceneManager()
{
    GameSystem::Instance().DeInit();

    if (now_scene) now_scene->DeInit();
    safe_delete(now_scene);

    if (next_scene) next_scene->DeInit();
    safe_delete(next_scene);
}


void SceneManager::Update(float elapsedTime)
{
    // �V�[�������I���̏ꍇ �^�C�g���Ɏ����ݒ�
    if (now_scene == nullptr)
    {
        next_scene = new Title();
    }


    // �V�[�����ς��Ƃ�
    ChangeScene();

    
    //�@�V�[���X�V
    now_scene->Update(elapsedTime);

}


void SceneManager::Render(float elapsedTime)
{

    //                                  //
    //              �`�揀��            �@//
    //                                  //

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // ��ʃN���A�������_�[�^�[�Q�b�g�ݒ�

#if 0
    FLOAT color[] = { 1.0f, 0.0f, 1.0f, 1.0f };	// RGBA(0.0�`1.0)
#else
    const DirectX::XMFLOAT4 back_color = now_scene->back_color;             // �V�[�����Ƃ̔w�i�F�ɕύX
    FLOAT color[] = { back_color.x,back_color.y,back_color.z,back_color.w };
#endif

    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // �`�揈��
    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j


    //�J�����p�����[�^�̐ݒ�
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();				//�r���[�s��̎擾
    rc.projection = camera.GetProjection();	//�v���W�F�N�V�����s��̎擾
    



    //                                      //
    //              ���C���`��                //
    //                                      //

    {

        // 3D�`��
        {
            Shader* shader = graphics.GetShader();
            shader->Begin(dc, rc);

            // �V�[���`��
            now_scene->ModelRender(dc, shader);

            shader->End(dc);
        }

        // �G�t�F�N�g�`��
        {
            EffectManager::Instance()->Render(rc.view, rc.projection);
        }


        // 2D�`��
        {
            now_scene->SpriteRender(dc);
        }

    }
    

    //                                      //
    //              �f�o�b�O�`��              //
    //                                      //

#if _DEBUG
    {
        // ImGui�`��
        ImGui();

        now_scene->DebugRender();

        // �f�o�b�O�����_���[�̕`��
        graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
    }
#endif

}


void SceneManager::Init()
{
    now_scene->Init(this);
}


void SceneManager::DeInit()
{
    if (now_scene == nullptr) return;

    now_scene->DeInit();
    safe_delete(now_scene);
}


void SceneManager::SetLoadNextScene(Scene* next_scene_)
{
    if (next_scene) return;

    next_scene = new Loading(next_scene_);
}


void SceneManager::SetNextScene(Scene* next_scene_)
{
    if (next_scene) return;

    next_scene = next_scene_;
}


void SceneManager::ImGui()
{
    {
        ImGui::Begin("ImGui");

        now_scene->ImGui();

        ImGui::End();
    }
}


void SceneManager::ChangeScene()
{
    if (next_scene == nullptr) return;

    DeInit();

    now_scene = next_scene;
    next_scene = nullptr;


    if (now_scene->IsSceneLoaded()) return;
    Init();
}