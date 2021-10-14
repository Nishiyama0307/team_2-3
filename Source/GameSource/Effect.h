#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>





class Effect
{
public:
    Effect(const char* filename);
    ~Effect();

    // çƒê∂
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);

    // í‚é~
    void Stop(Effekseer::Handle handle);

public:
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3 position);
    void SetScale(Effekseer::Handle handle, const float scale);

private:
    Effekseer::Effect* effekseerEffect = nullptr;

};