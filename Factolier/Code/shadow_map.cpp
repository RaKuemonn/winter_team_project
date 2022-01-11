
#include "shadow_map.h"
#include "texture.h"
#include "imgui.h"
#include "utility.h"



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



	//定数バッファオブジェクトの生成

	//シーンバッファ
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(Scene_Constant);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	hr = device->CreateBuffer(&buffer_desc, nullptr, scene_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	// サンプラステートの生成
	D3D11_SAMPLER_DESC sampler_desc{};
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	sampler_desc.MipLODBias = 0;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampler_desc.BorderColor[0] = FLT_MAX;
	sampler_desc.BorderColor[1] = FLT_MAX;
	sampler_desc.BorderColor[2] = FLT_MAX;
	sampler_desc.BorderColor[3] = FLT_MAX;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampler_desc, shadow_sampler_state.GetAddressOf());




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


	create_vs_from_cso(device, "./CSO/shadow_map_vs.cso",
		vertex_shader.GetAddressOf(),
		input_layout.GetAddressOf(),
		input_element_desc, ARRAYSIZE(input_element_desc));

}


void Shadow_Map::begin(ID3D11DeviceContext* immediate_context, float elapsed_time)
{
	//リソースのリセット
	ID3D11ShaderResourceView* clear_shader_resource_view[]{ nullptr };
	immediate_context->PSSetShaderResources(2, 1, clear_shader_resource_view);

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
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediate_context->IASetInputLayout(input_layout.Get());
	immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(nullptr, nullptr, 0);


	// ライトの位置から見た視線行列を生成
	Light* light = Light_Manager::instance().get_light(Lights::DEFAULT);

	DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat4(&light->light_position);
	LightPosition = DirectX::XMVectorScale(LightPosition, 1);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
		LightPosition + XMLoadFloat4(&light->light_direction),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// シャドウマップに描画したい範囲の正射影行列を生成
	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(SHADOW_DRAWRECT, SHADOW_DRAWRECT,
		0.1f, 200.0f);



	//定数バッファの更新
	DirectX::XMFLOAT4X4 light_view_projection;
	DirectX::XMStoreFloat4x4(&light_view_projection, V * P);

	Scene_Constant scene{};
	scene.view_projection = light_view_projection;
	immediate_context->UpdateSubresource(scene_buffer.Get(), 0, 0, &scene, 0, 0);
	immediate_context->VSSetConstantBuffers(1, 1, scene_buffer.GetAddressOf());
	immediate_context->PSSetConstantBuffers(1, 1, scene_buffer.GetAddressOf());

}


void Shadow_Map::end(ID3D11DeviceContext* immediate_context)
{
	//レンダーターゲットビューの初期化
	immediate_context->OMSetRenderTargets(0, nullptr, nullptr);


	//ビューポートの再設定
	D3D11_VIEWPORT viewport{};

	viewport.TopLeftX = 0;									//画面左上のx座標
	viewport.TopLeftY = 0;									//画面左上のy座標
	viewport.Width = static_cast<float>(SCREEN_WIDTH);		//画面の横サイズ
	viewport.Height = static_cast<float>(SCREEN_HEIGHT);	//画面の縦サイズ
	viewport.MinDepth = 0.0f;								//深度値の最小値
	viewport.MaxDepth = 1.0f;								//深度値の最大値

	immediate_context->RSSetViewports(1, &viewport);		//ビューポートを設定


	//シャドウマップテクスチャのバインド
	immediate_context->PSSetShaderResources(2, 1, shadow_shader_resource_view.GetAddressOf());
	immediate_context->PSSetSamplers(4, 1, shadow_sampler_state.GetAddressOf());


	ImGui::Begin("ImGui");
	
	ImGui::Separator();
	if (ImGui::TreeNode("texture"))
	{
		
		ImGui::Text("shadow_map");
		ImGui::Image(shadow_shader_resource_view.Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });
	
		ImGui::TreePop();
	}
	
	ImGui::End();
}


