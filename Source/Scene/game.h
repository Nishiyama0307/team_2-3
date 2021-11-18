#pragma once

#include "scene.h"
#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include <memory>

// 追加
#include "Charactor/Player.h"
#include "Charactor/EnemyManager.h"
#include "Charactor/Enemy_Arrangement.h"
#include "pause.h"
#include "coutdown.h"
#include "C_function.h"
#include "CameraController.h"
#include "stage.h"


enum STAGENUM
{
    N_STAGE1_VOLCANO,
    N_STAGE2_DESERT,
    N_STAGE3_,
    N_STAGE4_,
};

// ゲームシーン

constexpr int kStage2_Start_Position{ 565 };
constexpr int kStage3_Start_Position{ 1242 };
constexpr int kStage4_Start_Position{ 1920 };

class Game : public Scene
{
public:
    Game() {}
    ~Game()override{}

    void Update(float elapsedTime)override;
    void ModelRender(ID3D11DeviceContext* dc, Shader* shader)override;
    void SpriteRender(ID3D11DeviceContext* dc)override;

    void DeInit()override;

    void Set()override;
    void Load()override;

    void ImGui()override;

    void DebugRender()override;

private:
    void StageNumUpdate();
    void CameraSet();
    void ChangeScene(float elapsedTime);
    void ClearedSpriteRender(ID3D11DeviceContext* dc);
    void BGMStart();

private:
    std::unique_ptr<Pause> pause = nullptr;
    std::unique_ptr<CountDown> countdown = nullptr;

    Player* player = nullptr;
    Enemy_Arrangement* enemy_Arrangement = nullptr;

    // 黒帯
    std::unique_ptr<Sprite> black_band = nullptr;
    const float black_band_timer_max = 1.4f;
    float black_band_timer = 0.0f;

    bool smallest = false;

    Pos attacSelectPos;

    float stress = 0;

    STAGENUM stage_num;
    
    CameraController* cameraController = nullptr;

    //UI達
#if 1
    std::unique_ptr<Sprite> HpBar               = nullptr;      //HP
    std::unique_ptr<Sprite> HpBarFrame          = nullptr;      //
    std::unique_ptr<Sprite> HpIcon              = nullptr;      //
    std::unique_ptr<Sprite> StressBar           = nullptr;      //ストレス
    std::unique_ptr<Sprite> StressBarFrame      = nullptr;      //
    std::unique_ptr<Sprite> StressIcon          = nullptr;      //
    std::unique_ptr<Sprite> GoldBar             = nullptr;      //お金
    std::unique_ptr<Sprite> GoldIcon            = nullptr;      //お金
    std::unique_ptr<Sprite> Minimap             = nullptr;      //ミニマップ
    std::unique_ptr<Sprite> Castlebar           = nullptr;      //城
    std::unique_ptr<Sprite> CastlebarFrame      = nullptr;      //
    std::unique_ptr<Sprite> CastleIcon          = nullptr;      //
    std::unique_ptr<Sprite> AttackSlot1         = nullptr;      //攻撃スロット1
    std::unique_ptr<Sprite> AttackSlot2         = nullptr;      //攻撃スロット2
    std::unique_ptr<Sprite> AttackSlot3         = nullptr;      //攻撃スロット3
    std::unique_ptr<Sprite> AttackSelect        = nullptr;      //攻撃選択
    std::unique_ptr<Sprite> Minimap_Brave       = nullptr;      //ミニマップの勇者アイコン
    std::unique_ptr<Sprite> Minimap_Player      = nullptr;      //ミニマップのプレイヤーアイコン


	float AttackSlotScale1 = 1.0f;
    float AttackSlotScale2 = 1.0f;
    float AttackSlotScale3 = 1.0f;
    float attackPosx1 = 0;
    float attackPosx2 = 0;
    float attackPosx3 = 0;
    float attackPosY1 = 0;
    float attackPosY2 = 0;
    float attackPosY3 = 0;
    float Minimap_Brave_angle{ DirectX::XMConvertToRadians(0) };
	float Minimap_Player_angle{ DirectX::XMConvertToRadians(180) };
    float CastleHP = 0;

    bool brave_timer_count{ false };
    int brave_timer_{ 0 };
    int brave_timelimit_{ 5400 }; // 90 * 60

    //ゲームオーバー時フェードアウト
    std::unique_ptr<Sprite> spr_fadeOut = nullptr;
    float fade_timer = 0;
    bool is_fadeSprite = false;
#endif

    // チュートリアル
#if 1
    void End_of_explanation(float elapsedTime);

    std::unique_ptr<Sprite> spr_explanation_[8]{ nullptr };
    std::unique_ptr<Sprite> spr_check_1_[4]{ nullptr };
    std::unique_ptr<Sprite> spr_check_2_[2]{ nullptr };
    std::unique_ptr<Sprite> spr_choice_arrow_[2]{ nullptr };

    //はい、いいえのマウス用確認
    std::unique_ptr<Sprite> spr_kakunin_frame = nullptr ;
    std::unique_ptr<Sprite> spr_kakunin_frame2 = nullptr ;
    std::unique_ptr<Sprite> spr_frame = nullptr;
    Pos kakuninPos;
    Pos kakuninPos2;
    Pos framePos;
    bool is_kakuninn;
    //マウスカーソル
    std::unique_ptr<Sprite> spr_mouseCursor = nullptr;
    Pos mousePos;
    C_Hitbox MouseBox;
    //当たり判定
    C_Hitbox RetryeBox;
    C_Hitbox GameBox;
    //当たりのチェック
    bool hit = false;
    bool Retry_hit = false;
    bool retry_check = false;
    bool game_check = false;
    //点滅
    int timer1 = 0;
    int timer2 = 0;

    int tutorial_state = 0;
    int explanation = 0;
    int check_timer = 0;

    float angle = 0.0f;
    float foot_length = 1.5f;

    bool explaining = true; // 説明中

    bool wasd_[4]{ false };
    bool jump_{ false };
    bool dash_{ false };
    bool camera_move_{ false };
    bool attack_[2]{ false };

    bool tutorial_retry_[2]{ false };

    enum tutorial_contents
    {
        PLAYER_MOVE,            // プレイヤー移動操作＋ジャンプ＋ダッシュ＋カメラ操作
        ATTACK_MOVE,            // 攻撃
        END,                    // 終了
    };
#endif


};