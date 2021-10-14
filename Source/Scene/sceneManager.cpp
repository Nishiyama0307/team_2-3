
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
    // シーンが未選択の場合 タイトルに自動設定
    if (now_scene == nullptr)
    {
        next_scene = new Title();
    }


    // シーンが変わるとき
    ChangeScene();

    
    //　シーン更新
    now_scene->Update(elapsedTime);

}


void SceneManager::Render(float elapsedTime)
{

    //                                  //
    //              描画準備            　//
    //                                  //

    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

    // 画面クリア＆レンダーターゲット設定

#if 0
    FLOAT color[] = { 1.0f, 0.0f, 1.0f, 1.0f };	// RGBA(0.0～1.0)
#else
    const DirectX::XMFLOAT4 back_color = now_scene->back_color;             // シーンごとの背景色に変更
    FLOAT color[] = { back_color.x,back_color.y,back_color.z,back_color.w };
#endif

    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);

    // 描画処理
    RenderContext rc;
    rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）


    //カメラパラメータの設定
    Camera& camera = Camera::Instance();
    rc.view = camera.GetView();				//ビュー行列の取得
    rc.projection = camera.GetProjection();	//プロジェクション行列の取得
    



    //                                      //
    //              メイン描画                //
    //                                      //

    {

        // 3D描画
        {
            Shader* shader = graphics.GetShader();
            shader->Begin(dc, rc);

            // シーン描画
            now_scene->ModelRender(dc, shader);

            shader->End(dc);
        }

        // エフェクト描画
        {
            EffectManager::Instance()->Render(rc.view, rc.projection);
        }


        // 2D描画
        {
            now_scene->SpriteRender(dc);
        }

    }
    

    //                                      //
    //              デバッグ描画              //
    //                                      //

#if _DEBUG
    {
        // ImGui描画
        ImGui();

        now_scene->DebugRender();

        // デバッグレンダラーの描画
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