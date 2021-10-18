#include "Enemy.h"
#include "EnemyManager.h"
#include "Graphics/Graphics.h"
#include "Player.h"

// デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	
	// 当たり判定の大きさ
	radius = 1.0f;

	height = 6.0f;
	
	// 衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(hit_position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));

	// 衝突判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}


void Enemy::UpdateVelocity(float elapsedTime, int kind)
{

	float elapsedFrame = 60.0f * elapsedTime;

	UpdateVerticalVelocity(elapsedFrame, kind);

	UpdateVerticalMove(elapsedTime, kind);


	if (Is_inhaling == false) return;


}

// 破棄
void Enemy::Destroy()
{
	EnemyManager::Instance().Remove(this);
}