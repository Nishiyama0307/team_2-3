#pragma once

#include "Actor/transform.h"
class Player;


// 吸い込みクラス ※ Playerクラスに依存

class Inhale
{
public:
    Inhale(Player* player_);
    ~Inhale() {};

public:
    void Update(float elapsedTime);
    void DebugRender();

    void InputInhale();

public: // Get関数
    float GetHitRadius() { return radius; }
    bool IsDuringInhale() { return is_during_inhale; }

public: // Set関数

private:
    void UpdateNozzlePosition();                                        // ノズルの位置の更新用
    void Collision();                                                   // 当たり判定
    void KnockBack(DirectX::XMFLOAT3 pos_a, DirectX::XMFLOAT3 pos_b);   // ノックバック処理


private:
    Player* player = nullptr;           // プレイヤーの情報を引用する用
    bool is_during_inhale = false;      // 吸っているか
    Transform nozzle = {};              // 姿勢表現ようの変数群
    float radius = 1.0f;                // 当たり判定(球)の半径
};
