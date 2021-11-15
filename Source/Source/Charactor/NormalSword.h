#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class NormalSword : public Enemy
{
public:
	NormalSword();
	~NormalSword() override;

	//void Update(float elapsedTime) override;
	void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	DirectX::XMFLOAT3 inhaled() override;


	//// �v���C���[�ɑ΂��Ă̒ǔ�����
	//void HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	void SetInhaleParameter();
	void SetParameter(Parameter param) { this->par = param; };


private:
	//Model* model = nullptr;
};