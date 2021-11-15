#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class NormalSpear : public Enemy
{
public:
	NormalSpear();
	~NormalSpear() override;

	void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos , int stage_num) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	DirectX::XMFLOAT3 inhaled() override;

	void SetInhaleParameter();
	void SetParameter(Parameter param) { this->par = param; };


private:
	//Model* model = nullptr;
};