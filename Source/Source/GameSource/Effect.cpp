
#include "Graphics/Graphics.h"
#include "EffectManager.h"
#include "Effect.h"



Effect::Effect(const char* filename)
{

    // リーソースの読み込み
    // (UTF-16のファイルパスに変換)
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    Effekseer::Manager* effekseerM = EffectManager::Instance()->GetEffekseerManager();

    // 読み込んだエフェクトの生成
    effekseerEffect = Effekseer::Effect::Create(effekseerM, (EFK_CHAR*)utf16Filename);

}


Effect::~Effect()
{
    if (effekseerEffect != nullptr)
    {
        effekseerEffect->Release();
        effekseerEffect = nullptr;
    }
}


Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
    Effekseer::Manager* effekseerM = EffectManager::Instance()->GetEffekseerManager();

    Effekseer::Handle handle = effekseerM->Play(effekseerEffect, position.x, position.y, position.z);
    effekseerM->SetScale(handle, scale, scale, scale);

    return handle;
}


void Effect::Stop(Effekseer::Handle handle)
{
    Effekseer::Manager* effekseerM = EffectManager::Instance()->GetEffekseerManager();

    effekseerM->StopEffect(handle);
}


void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3 position)
{
    Effekseer::Manager* effekseerM = EffectManager::Instance()->GetEffekseerManager();

    effekseerM->SetLocation(handle, position.x, position.y, position.z);
}


void Effect::SetScale(Effekseer::Handle handle, const float scale)
{
    Effekseer::Manager* effekseerM = EffectManager::Instance()->GetEffekseerManager();

    effekseerM->SetScale(handle, scale, scale, scale);
}