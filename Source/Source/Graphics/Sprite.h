#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

// スプライト
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

	// 描画実行 1
	void Render(ID3D11DeviceContext *dc,
		float dx, float dy,							// 描画位置
		float dw, float dh,							// 描画サイズ
		float sx, float sy,							// 元画像位置
		float sw, float sh,							// 表示幅,高さ
		float angle,								// 角度
		float r, float g, float b, float a) const;	// 色情報(r,g,b,a)
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

	// 描画実行 2
	void Render2(ID3D11DeviceContext* dc,
		DirectX::XMFLOAT2 pos,			// 表示位置
		DirectX::XMFLOAT2 scale,		// スケール
		DirectX::XMFLOAT2 texPos,		// 画像切り抜き位置
		DirectX::XMFLOAT2 texSize,		// 画像切り抜きサイズ
		DirectX::XMFLOAT2 center,		// 画像基準点
		float angle,					// 角度
		DirectX::XMFLOAT4 color) const;	// 色情報(r,g,b,a)
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


	// テキストアウト
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

	// 数値限定のテキストアウト
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

	// テクスチャ幅取得
	int GetTextureWidth() const { return textureWidth; }

	// テクスチャ高さ取得
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