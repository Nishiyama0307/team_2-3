#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

// �X�v���C�g
class Sprite
{
public:
	Sprite();
	Sprite(const char* filename);
	~Sprite() {}

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	// �`����s 1
	void Render(ID3D11DeviceContext *dc,
		float dx, float dy,							// �`��ʒu
		float dw, float dh,							// �`��T�C�Y
		float sx, float sy,							// ���摜�ʒu
		float sw, float sh,							// �\����,����
		float angle,								// �p�x
		float r, float g, float b, float a) const;	// �F���(r,g,b,a)
	void Render(ID3D11DeviceContext* dc,
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT2 texPos,
		DirectX::XMFLOAT2 texSize,
		float angle,
		DirectX::XMFLOAT4 color) const
	{
		Render(dc, pos.x, pos.y, size.x, size.y, texPos.x, texPos.y, texSize.x, texSize.y, angle, color.x, color.y, color.z, color.w);
	}

	// �`����s 2
	void Render2(ID3D11DeviceContext* dc,
		DirectX::XMFLOAT2 pos,			// �\���ʒu
		DirectX::XMFLOAT2 scale,		// �X�P�[��
		DirectX::XMFLOAT2 texPos,		// �摜�؂蔲���ʒu
		DirectX::XMFLOAT2 texSize,		// �摜�؂蔲���T�C�Y
		DirectX::XMFLOAT2 center,		// �摜��_
		float angle,					// �p�x
		DirectX::XMFLOAT4 color) const;	// �F���(r,g,b,a)
	void Render2(ID3D11DeviceContext* dc,
		float pos_x, float pos_y,					
		float scale_x, float scale_y,
		float tex_pos_x, float tex_pos_y,
		float tex_size_x, float tex_size_y,
		float center_x, float center_y,
		float angle,
		float r, float g, float b, float a) const
	{
		Render2(dc, { pos_x,pos_y }, { scale_x,scale_y }, { tex_pos_x,tex_pos_y }, { tex_size_x,tex_size_y }, { center_x,center_y }, angle, { r,g,b,a });
	}


	// �e�L�X�g�A�E�g
	void Textout(ID3D11DeviceContext* dc,
		std::string str,
		float pos_x, float pos_y,
		float size_x, float size_y,
		float r, float g, float b, float a) const;
	void Textout(ID3D11DeviceContext* dc,
		std::string str,
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT4 color) const
	{
		Textout(dc, str, pos.x, pos.y, size.x, size.y, color.x, color.y, color.z, color.w);
	}

	// ���l����̃e�L�X�g�A�E�g
	void Textout_Number(ID3D11DeviceContext* dc,
		int number,
		float pos_x, float pos_y,
		float size_x, float size_y,
		float r, float g, float b, float a) const;
	void Textout_Number(ID3D11DeviceContext* dc,
		int number,
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT4 color) const
	{
		Textout_Number(dc, number, pos.x, pos.y, size.x, size.y, color.x, color.y, color.z, color.w);
	}

	// �e�N�X�`�����擾
	int GetTextureWidth() const { return textureWidth; }

	// �e�N�X�`�������擾
	int GetTextureHeight() const { return textureHeight; }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;

	int textureWidth = 0;
	int textureHeight = 0;
};