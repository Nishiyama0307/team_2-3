#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include "Graphics/ModelResource.h"

// ���f��
class Model
{
public:
	Model(const char* filename);
	~Model() {}

	struct Node
	{
		const char*			name;
		Node*				parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	localTransform;
		DirectX::XMFLOAT4X4	worldTransform;

		std::vector<Node*>	children;
	};

	// �s��v�Z
	void UpdateTransform(const DirectX::XMFLOAT4X4& transform);

	// �m�[�h���X�g�擾
	const std::vector<Node>& GetNodes() const { return nodes; }
	std::vector<Node>& GetNodes() { return nodes; }

	// ���\�[�X�擾
	const ModelResource* GetResource() const { return resource.get(); }

	int GetIndex() const { return currentAnimationIndex; }

	// �A�j���[�V�����X�V����
	void UpdateAnimation(float elapsedTime);

	// �A�j���[�V�����Đ�
	void PlayAnimation(int index, bool loop, float blendSeconds = 0.2f);

	// �A�j���[�V�����Đ�����
	bool IsPlayAnimation() const;

private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	int		currentAnimationIndex = -1;
	float	currentAnimationSeconds = 0.0f;
	bool	animationLoopFlag = false;
	bool	animationEndFlag = false;
	float	animationBlendTime = 0.0f;
	float	animationBlendSeconds = 0.0f;
};