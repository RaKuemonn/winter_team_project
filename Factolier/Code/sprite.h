#pragma once

#include <d3d11.h>
#include <Directxmath.h>
#include "texture.h"
#include "shader.h"

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 texcoord;
};


class Sprite
{
public:
	Sprite(ID3D11Device* device, const char* filename);
	~Sprite();

	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;
	ID3D11InputLayout* input_layout;
	ID3D11Buffer* vertex_buffer;
	ID3D11ShaderResourceView* shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;


	void render(ID3D11DeviceContext* immediate_context,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float px, float py,
		float r, float g, float b, float a,
		float angle/*degree*/);

	Vertex* Screen_to_NDC(ID3D11DeviceContext* immediate_context,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float px, float py,
		float r, float g, float b, float a,
		float angle/*degree*/);

	void Bind(ID3D11DeviceContext* immediate_context);
};

