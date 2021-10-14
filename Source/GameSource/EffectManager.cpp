

#include "Graphics/Graphics.h"
#include "EffectManager.h"



void EffectManager::Initialize()
{
    Graphics& graphic = Graphics::Instance();

    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphic.GetDevice(), graphic.GetDeviceContext(), 2048);
    effekseerManager = Effekseer::Manager::Create(2048);

    // Effekseerの設定
    {
#define SetRenderer(field)  effekseerManager->Set##field(effekseerRenderer->Create##field());

        // レンダラの設定
        SetRenderer(SpriteRenderer);
        SetRenderer(RibbonRenderer);
        SetRenderer(RingRenderer);
        SetRenderer(TrackRenderer);
        SetRenderer(ModelRenderer);
#undef SetRenderer


#define SetLoader(field)  effekseerManager->Set##field(effekseerRenderer->Create##field());

        // ローダーの設定
        SetLoader(TextureLoader);
        SetLoader(ModelLoader);
        SetLoader(MaterialLoader);
#undef SetLoader

        // Effekseerを左手座標系で計算
        effekseerManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

    }

}


void EffectManager::Finalize()
{
    if (effekseerManager != nullptr)
    {
        effekseerManager->Destroy();
        effekseerManager = nullptr;
    }

    if (effekseerRenderer != nullptr)
    {
        effekseerRenderer->Destroy();
        effekseerRenderer = nullptr;
    }
}


void EffectManager::Update(float elapsedTime)
{
    // エフェクトの更新 (フレームの経過時間を引数に渡す)
    effekseerManager->Update(elapsedTime * 60.0f);
}


void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{

    // ビュー＆プロジェクション行列を Effekseerレンダラに設定
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    // 描画開始
    effekseerRenderer->BeginRendering();

    // 描画実行
    effekseerManager->Draw();

    // 描画終了
    effekseerRenderer->EndRendering();


}