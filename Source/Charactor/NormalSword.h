#pragma once

#include "Graphics/Model.h"
#include "Enemy.h"

class NormalSword : public Enemy
{
public:
	NormalSword();
	~NormalSword() override;

	//void Update(float elapsedTime) override;
	void Update(float elapsedTime, const DirectX::XMFLOAT3& playerPos, int stage_num) override;
	void Render(ID3D11DeviceContext* dc, Shader* shader) override;

	//// ƒvƒŒƒCƒ„[‚É‘Î‚µ‚Ä‚Ì’Ç”ö“®ì
	//void HomingMove(float elapsedTime, const DirectX::XMFLOAT3& playerPos);

	void SetParameter(Parameter param) { this->par = param; };


private:
	//Model* model = nullptr;
};