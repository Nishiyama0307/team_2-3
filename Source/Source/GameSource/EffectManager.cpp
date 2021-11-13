

#include "Graphics/Graphics.h"
#include "EffectManager.h"



void EffectManager::Initialize()
{
    Graphics& graphic = Graphics::Instance();

    effekseerRenderer = EffekseerRendererDX11::Renderer::Create(graphic.GetDevice(), graphic.GetDeviceContext(), 2048);
    effekseerManager = Effekseer::Manager::Create(2048);

    // Effekseer�̐ݒ�
    {
#define SetRenderer(field)  effekseerManager->Set##field(effekseerRenderer->Create##field());

        // �����_���̐ݒ�
        SetRenderer(SpriteRenderer);
        SetRenderer(RibbonRenderer);
        SetRenderer(RingRenderer);
        SetRenderer(TrackRenderer);
        SetRenderer(ModelRenderer);
#undef SetRenderer


#define SetLoader(field)  effekseerManager->Set##field(effekseerRenderer->Create##field());

        // ���[�_�[�̐ݒ�
        SetLoader(TextureLoader);
        SetLoader(ModelLoader);
        SetLoader(MaterialLoader);
#undef SetLoader

        // Effekseer��������W�n�Ōv�Z
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
    // �G�t�F�N�g�̍X�V (�t���[���̌o�ߎ��Ԃ������ɓn��)
    effekseerManager->Update(elapsedTime * 60.0f);
}


void EffectManager::Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{

    // �r���[���v���W�F�N�V�����s��� Effekseer�����_���ɐݒ�
    effekseerRenderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&view));
    effekseerRenderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&projection));

    // �`��J�n
    effekseerRenderer->BeginRendering();

    // �`����s
    effekseerManager->Draw();

    // �`��I��
    effekseerRenderer->EndRendering();


}