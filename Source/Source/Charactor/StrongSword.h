#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class StrongSword : public Enemy
{
public:
	StrongSword();
	~StrongSword() override;

	void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos = { 0,0,0 }) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	DirectX::XMFLOAT3 inhaled() override;

	void SetInhaleParameter();
	void SetParameter(Parameter param) { this->par = param; };

private:
	//Model* model = nullptr;
};