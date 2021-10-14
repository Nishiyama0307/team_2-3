#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class RareEnemy : public Enemy
{
public:
	RareEnemy();
	~RareEnemy() override;

	void Update(float elapsedTime) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	DirectX::XMFLOAT3 inhaled() override;

	void SetInhaleParameter();

private:
	Model* model = nullptr;
};