#include <stdio.h> 
#include <WICTextureLoader.h>
#include <algorithm>
#include "Sprite.h"
#include "Misc.h"
#include "Graphics/Graphics.h"

// コンストラクタ
Sprite::Sprite()
	: Sprite(nullptr)
{
}

// コンストラクタ
Sprite::Sprite(const char* filename)
{
	ID3D11Device* device = Graphics::Instance().GetDevice();

	HRESULT hr = S_OK;

	// 頂点データの定義
	// 0           1
	// +-----------+
	// |           |
	// |           |
	// +-----------+
	// 2           3
	Vertex vertices[] = {
		{ DirectX::XMFLOAT3(-0.5, +0.5, 0), DirectX::XMFLOAT4(1, 1, 1, 1) },
		{ DirectX::XMFLOAT3(+0.5, +0.5, 0), DirectX::XMFLOAT4(1, 0, 0, 1) },
		{ DirectX::XMFLOAT3(-0.5, -0.5, 0), DirectX::XMFLOAT4(0, 1, 0, 1) },
		{ DirectX::XMFLOAT3(+0.5, -0.5, 0), DirectX::XMFLOAT4(0, 0, 1, 1) },
	};

	// ポリゴンを描画するにはGPUに頂点データやシェーダーなどのデータを渡す必要がある。
	// GPUにデータを渡すにはID3D11***のオブジェクトを介してデータを渡します。

	// 頂点バッファの生成
	{
		// 頂点バッファを作成するための設定オプション
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(vertices);	// バッファ（データを格納する入れ物）のサイズ
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;	// UNIT.03
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファとしてバッファを作成する。
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// UNIT.03
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		// 頂点バッファに頂点データを入れるための設定
		D3D11_SUBRESOURCE_DATA subresource_data = {};
		subresource_data.pSysMem = vertices;	// ここに格納したい頂点データのアドレスを渡すことでCreateBuffer()時にデータを入れることができる。
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.
		// 頂点バッファオブジェクトの生成
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &vertexBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\SpriteVS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// 頂点シェーダー生成
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, inputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Shader\\SpritePS.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blendState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = false;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// サンプラステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		HRESULT hr = device->CreateSamplerState(&desc, samplerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}

	// テクスチャの生成
	if (filename != nullptr)
	{
		// マルチバイト文字からワイド文字へ変換
		wchar_t wfilename[256];
		::MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, 256);

		// テクスチャファイル読み込み
		// テクスチャ読み込み
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfilename, resource.GetAddressOf(), shaderResourceView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// テクスチャ情報の取得
		D3D11_TEXTURE2D_DESC desc;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
		hr = resource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		texture2d->GetDesc(&desc);

		textureWidth = desc.Width;
		textureHeight = desc.Height;
	}
	else
	{
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));
		data.pSysMem = pixels;
		data.SysMemPitch = width;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
		HRESULT hr = device->CreateTexture2D(&desc, &data, texture.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		hr = device->CreateShaderResourceView(texture.Get(), nullptr, shaderResourceView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		textureWidth = desc.Width;
		textureHeight = desc.Height;
	}
}

// 描画実行
void Sprite::Render(ID3D11DeviceContext *immediate_context,
	float dx, float dy,		/* 描画位置	 */
	float dw, float dh,		/* 描画サイズ	 */
	float sx, float sy,		/* 元画像位置	 */
	float sw, float sh,		/* 表示幅,高さ */
	float angle,
	float r, float g, float b, float a) const
{
	{
		// 現在設定されているビューポートからスクリーンサイズを取得する。
		D3D11_VIEWPORT viewport;
		UINT numViewports = 1;
		immediate_context->RSGetViewports(&numViewports, &viewport);
		float screen_width = viewport.Width;
		float screen_height = viewport.Height;

		// スプライトを構成する４頂点のスクリーン座標を計算する
		DirectX::XMFLOAT2 positions[] = {
			DirectX::XMFLOAT2(dx,      dy),			// 左上
			DirectX::XMFLOAT2(dx + dw, dy),			// 右上
			DirectX::XMFLOAT2(dx,      dy + dh),	// 左下
			DirectX::XMFLOAT2(dx + dw, dy + dh),	// 右下
		};

		// スプライトを構成する４頂点のテクスチャ座標を計算する
		DirectX::XMFLOAT2 texcoords[] = {
			DirectX::XMFLOAT2(sx,      sy),			// 左上
			DirectX::XMFLOAT2(sx + sw, sy),			// 右上
			DirectX::XMFLOAT2(sx,      sy + sh),	// 左下
			DirectX::XMFLOAT2(sx + sw, sy + sh),	// 右下
		};

		// スプライトの中心で回転させるために４頂点の中心位置が
		// 原点(0, 0)になるように一旦頂点を移動させる。
		float mx = dx + dw * 0.5f;
		float my = dy + dh * 0.5f;
		for (auto& p : positions)
		{
			p.x -= mx;
			p.y -= my;
		}

		// 頂点を回転させる
		const float PI = 3.141592653589793f;
		float theta = angle * (PI / 180.0f);	// 角度をラジアン(θ)に変換
		float c = cosf(theta);
		float s = sinf(theta);
		for (auto& p : positions)
		{
			DirectX::XMFLOAT2 r = p;
			p.x = c * r.x + -s * r.y;
			p.y = s * r.x + c * r.y;
		}

		// 回転のために移動させた頂点を元の位置に戻す
		for (auto& p : positions)
		{
			p.x += mx;
			p.y += my;
		}

		// スクリーン座標系からNDC座標系へ変換する。
		for (auto& p : positions)
		{
			p.x = 2.0f*p.x / screen_width - 1.0f;
			p.y = 1.0f - 2.0f*p.y / screen_height;
		}

		// 頂点バッファの内容の編集を開始する。
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		HRESULT hr = immediate_context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		// pDataを編集することで頂点データの内容を書き換えることができる。
		Vertex* v = static_cast<Vertex*>(mappedBuffer.pData);
		for (int i = 0; i < 4; ++i)
		{
			v[i].position.x = positions[i].x;
			v[i].position.y = positions[i].y;
			v[i].position.z = 0.0f;

			v[i].color.x = r;
			v[i].color.y = g;
			v[i].color.z = b;
			v[i].color.w = a;

			v[i].texcoord.x = texcoords[i].x / textureWidth;
			v[i].texcoord.y = texcoords[i].y / textureHeight;
		}

		// 頂点バッファの内容の編集を終了する。
		immediate_context->Unmap(vertexBuffer.Get(), 0);
	}

	{
		// パイプライン設定
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		immediate_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		immediate_context->IASetInputLayout(inputLayout.Get());

		immediate_context->RSSetState(rasterizerState.Get());

		immediate_context->VSSetShader(vertexShader.Get(), nullptr, 0);
		immediate_context->PSSetShader(pixelShader.Get(), nullptr, 0);

		immediate_context->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
		immediate_context->PSSetSamplers(0, 1, samplerState.GetAddressOf());

		immediate_context->OMSetDepthStencilState(depthStencilState.Get(), 1);
		immediate_context->OMSetBlendState(blendState.Get(), nullptr, 0xFFFFFFFF);

		// 描画
		immediate_context->Draw(4, 0);
	}
}


void Sprite::Render2(ID3D11DeviceContext* dc,
	DirectX::XMFLOAT2 pos,
	DirectX::XMFLOAT2 scale,
	DirectX::XMFLOAT2 texPos,
	DirectX::XMFLOAT2 texSize,
	DirectX::XMFLOAT2 center,
	float angle,
	DirectX::XMFLOAT4 color) const
{
	if (scale.x == 0.0f || scale.y == 0.0f) return;

	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	float tw = texSize.x;
	float th = texSize.y;
	if (tw == FLT_MIN && th == FLT_MIN)
	{
		tw = static_cast<float>(textureWidth);
		th = static_cast<float>(textureHeight);
	}

	Vertex vertices[4] = {};
	vertices[0] = { {0.0f, 1.0f, 0}, color, {0, 1} };
	vertices[1] = { {1.0f, 1.0f, 0}, color, {1, 1} };
	vertices[2] = { {0.0f, 0.0f, 0}, color, {0, 0} };
	vertices[3] = { {1.0f, 0.0f, 0}, color, {1, 0} };


	float sinValue = sinf(angle);
	float cosValue = cosf(angle);
	float mx = (tw * scale.x) / tw * center.x;
	float my = (th * scale.y) / th * center.y;
	for (int i = 0; i < 4; i++)
	{
		vertices[i].position.x *= (tw * scale.x);
		vertices[i].position.y *= (th * scale.y);

		vertices[i].position.x -= mx;
		vertices[i].position.y -= my;

		float rx = vertices[i].position.x;
		float ry = vertices[i].position.y;
		vertices[i].position.x = rx * cosValue - ry * sinValue;
		vertices[i].position.y = rx * sinValue + ry * cosValue;

		vertices[i].position.x += mx;
		vertices[i].position.y += my;

		vertices[i].position.x += (pos.x - scale.x * center.x);
		vertices[i].position.y += (pos.y - scale.y * center.y);

		vertices[i].position.x = -1.0f + vertices[i].position.x * 2 / viewport.Width;
		vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;

		// UV座標の調整
		vertices[i].texcoord.x = (std::min)(vertices[i].texcoord.x, 1.0f);
		vertices[i].texcoord.y = (std::min)(vertices[i].texcoord.y, 1.0f);

		vertices[i].texcoord.x = (texPos.x + vertices[i].texcoord.x * tw) / textureWidth;
		vertices[i].texcoord.y = (texPos.y + vertices[i].texcoord.y * th) / textureHeight;
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertices, sizeof(vertices));
	dc->Unmap(vertexBuffer.Get(), 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	dc->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	dc->IASetInputLayout(inputLayout.Get());
	dc->RSSetState(rasterizerState.Get());
	dc->VSSetShader(vertexShader.Get(), nullptr, 0);
	dc->PSSetShader(pixelShader.Get(), nullptr, 0);

	dc->PSSetShaderResources(0, 1, shaderResourceView.GetAddressOf());
	dc->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	dc->OMSetDepthStencilState(depthStencilState.Get(), 1);

	dc->Draw(4, 0);
}


void Sprite::Textout(ID3D11DeviceContext* dc,
	std::string str,
	float pos_x, float pos_y,
	float size_x, float size_y,
	float r, float g, float b, float a) const
{
	// 1文字の幅と高さを計算
	float sw = static_cast<float>(textureWidth / 16);
	float sh = static_cast<float>(textureHeight / 16);

	// 現在の文字位置(相対位置)
	float carriage = 0;
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(blendState.Get(), blend_factor, 0xFFFFFFFF);

	// 一文字づつRender()する
	for (const char c : str)
	{
		LONG sx = c % 0x0F;
		//文字を表示。アスキーコードの位置にある文字位置を切り抜いて表示
		Render(dc, pos_x + carriage, pos_y, size_x, size_y, sw * (c & 0x0F), sh * (c >> 4), sw, sh, 0, r, g, b, a);
		//文字位置を幅分ずらす
		carriage += size_x;
	}
}


void Sprite::Textout_Number(ID3D11DeviceContext* dc,
	int number,
	float pos_x, float pos_y,
	float size_x, float size_y,
	float r, float g, float b, float a) const
{
	//　1文字の幅と高さを計算
	float sw = static_cast<float>(textureWidth / 16);
	float sh = static_cast<float>(textureHeight / 16);

	// 現在の文字位置(相対位置)
	float carriage = 0;
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(blendState.Get(), blend_factor, 0xFFFFFFFF);

	// 数値を文字に変換
	std::string str_number = std::to_string(number);

	// 一文字づつRender()する
	for (const char c : str_number)
	{
		LONG sx = c % 0x0F;
		//文字を表示。アスキーコードの位置にある文字位置を切り抜いて表示
		Render(dc, pos_x + carriage, pos_y, size_x, size_y, sw * (c & 0x0F), sh * (c >> 4), sw, sh, 0, r, g, b, a);
		//文字位置を幅分ずらす
		carriage += size_x;
	}
}