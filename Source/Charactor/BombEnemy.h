#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class BombEnemy : public Enemy
{
public:
	BombEnemy();
	~BombEnemy() override;

	void Update(float elapsedTime) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;


	DirectX::XMFLOAT3 inhaled() override;

	void SetInhaleParameter();

protected:
	// ���S�����Ƃ��ɌĂ΂��
	void OnDead() override;

private:
	Model* model = nullptr;
};