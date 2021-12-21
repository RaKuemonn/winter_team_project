
#include "shadow_map.h"
#include "texture.h"


CONST LONG SHADOW_WIDTH{ 1024 };
CONST LONG SHADOW_HEIGHT{ 1024 };
CONST float SHADOW_DRAWRECT{ 30 };



void Shadow_Map::initialize(ID3D11Device* device)
{
	HRESULT hr{ S_OK };

	// ライトから見たシーンの深度描画用のバッファ生成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_buffer{};
	D3D11_TEXTURE2D_DESC texture2d_desc{};

	texture2d_desc.Width = SHADOW_WIDTH;
	texture2d_desc.Height = SHADOW_HEIGHT;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_R32_TYPELESS;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;

	hr = device->CreateTexture2D(&texture2d_desc, NULL, depth_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//	深度ステンシルビュー生成
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};

	depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depth_buffer.Get(),
		&depth_stencil_view_desc,
		shadow_depth_stencil_view.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//	シェーダーリソースビュー生成
	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};

	shader_resource_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	shader_resource_view_desc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(depth_buffer.Get(),
		&shader_resource_view_desc,
		shadow_shader_resource_view.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	// シャドウマップ生成用シェーダー
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	create_vs_from_cso(device, "shadowmap_caster_vs.cso",
		vertex_shader.GetAddressOf(),
		input_layout.GetAddressOf(),
		input_element_desc, ARRAYSIZE(input_element_desc));

}


void Shadow_Map::begin(ID3D11DeviceContext* immediate_context, float elapsed_time)
{
	// シャドウマップ用の深度バッファに設定
	immediate_context->ClearDepthStencilView(shadow_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediate_context->OMSetRenderTargets(0, nullptr, shadow_depth_stencil_view.Get());
	// ビューポートの設定
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(SHADOW_WIDTH);
	viewport.Height = static_cast<float>(SHADOW_HEIGHT);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	immediate_context->RSSetViewports(1, &viewport);
	
	// シェーダー設定
	immediate_context->IASetInputLayout(input_layout.Get());
	immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(nullptr, nullptr, 0);

	DirectX::XMMATRIX S, R, T;
	DirectX::XMFLOAT4X4 world;
	// ライトの位置から見た視線行列を生成
	DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat4(&light_direction);
	LightPosition = DirectX::XMVectorScale(LightPosition, -50);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// シャドウマップに描画したい範囲の射影行列を生成
	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(SHADOW_DRAWRECT, SHADOW_DRAWRECT,
		0.1f, 200.0f);
}


void Shadow_Map::end(ID3D11DeviceContext* immediate_context)
{
    
}


