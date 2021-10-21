#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class NormalSword : public Enemy
{
public:
	NormalSword();
	~NormalSword() override;

	void Update(float elapsedTime) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	DirectX::XMFLOAT3 inhaled() override;

	void SetInhaleParameter();
	void SetParameter(Parameter param) { this->par = param; };

private:
	Model* model = nullptr;
};