#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class StrongSpear : public Enemy
{
public:
	StrongSpear();
	~StrongSpear() override;

	void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	void SetParameter(Parameter param) { this->par = param; };

private:
	//Model* model = nullptr;
};