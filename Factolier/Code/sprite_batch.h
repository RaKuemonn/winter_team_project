#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <Directxmath.h>
#include <vector>
#include "sprite.h"
#include "texture.h"
#include "shader.h"


//struct Vertex
//{
//	DirectX::XMFLOAT3 position;
//	DirectX::XMFLOAT4 color;
//	DirectX::XMFLOAT2 texcoord;
//};


class Sprite_Batch
{
public:
	Sprite_Batch(ID3D11Device* device, const char* filename, size_t max_sprites);
	~Sprite_Batch();

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;
	//Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;

	const size_t max_vertices;
	std::vector<Vertex> vertices;


	void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float px, float py,
		float r, float g, float b, float a,
		float angle/*degree*/);

	//void render(ID3D11DeviceContext* immediate_context,
	//		float dx, float dy, float dw, float dh,
	//		float r, float g, float b, float a,
	//		float angle/*degree*/);

	void begin(ID3D11DeviceContext* immediate_context);

	void end(ID3D11DeviceContext* immediate_context);


	//ì_(x, y)Ç™ì_(cx, cy)ÇíÜêSÇ…äp(angle)Ç≈âÒì]ÇµÇΩéûÇÃç¿ïWÇåvéZÇ∑ÇÈ
	void rotate(float& x, float& y, float cx, float cy, float& cos, float& sin)
	{
		x -= cx;
		y -= cy;

		float tx{ x }, ty{ y };
		x = cos * tx + -sin * ty;
		y = sin * tx + cos * ty;

		x += cx;
		y += cy;
	}
};


