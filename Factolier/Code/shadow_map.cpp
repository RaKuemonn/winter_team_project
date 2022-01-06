
#include "shadow_map.h"
#include "texture.h"
#include "imgui.h"
#include "utility.h"



void Shadow_Map::initialize(ID3D11Device* device)
{
	HRESULT hr{ S_OK };

	// ���C�g���猩���V�[���̐[�x�`��p�̃o�b�t�@����
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


	//	�[�x�X�e���V���r���[����
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};

	depth_stencil_view_desc.Format = DXGI_FORMAT_D32_FLOAT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depth_buffer.Get(),
		&depth_stencil_view_desc,
		shadow_depth_stencil_view.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//	�V�F�[�_�[���\�[�X�r���[����
	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};

	shader_resource_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
	shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_view_desc.Texture2D.MostDetailedMip = 0;
	shader_resource_view_desc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(depth_buffer.Get(),
		&shader_resource_view_desc,
		shadow_shader_resource_view.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	//�萔�o�b�t�@�I�u�W�F�N�g�̐���

	//�V�[���o�b�t�@
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(Scene_Constant);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	hr = device->CreateBuffer(&buffer_desc, nullptr, scene_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	// �T���v���X�e�[�g�̐���
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




	// �V���h�E�}�b�v�����p�V�F�[�_�[
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
	//���\�[�X�̃��Z�b�g
	ID3D11ShaderResourceView* clear_shader_resource_view[]{ nullptr };
	immediate_context->PSSetShaderResources(2, 1, clear_shader_resource_view);

	// �V���h�E�}�b�v�p�̐[�x�o�b�t�@�ɐݒ�
	immediate_context->ClearDepthStencilView(shadow_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	immediate_context->OMSetRenderTargets(0, nullptr, shadow_depth_stencil_view.Get());
	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(SHADOW_WIDTH);
	viewport.Height = static_cast<float>(SHADOW_HEIGHT);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	immediate_context->RSSetViewports(1, &viewport);
	
	// �V�F�[�_�[�ݒ�
	immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	immediate_context->IASetInputLayout(input_layout.Get());
	immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	immediate_context->PSSetShader(nullptr, nullptr, 0);


	// ���C�g�̈ʒu���猩�������s��𐶐�
	Light* light = Light_Manager::instance().get_light(Lights::DEFAULT);

	DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat4(&light->light_position);
	LightPosition = DirectX::XMVectorScale(LightPosition, 1);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookAtLH(LightPosition,
		LightPosition + XMLoadFloat4(&light->light_direction),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// �V���h�E�}�b�v�ɕ`�悵�����͈͂̐��ˉe�s��𐶐�
	DirectX::XMMATRIX P = DirectX::XMMatrixOrthographicLH(SHADOW_DRAWRECT, SHADOW_DRAWRECT,
		0.1f, 200.0f);



	//�萔�o�b�t�@�̍X�V
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
	//�����_�[�^�[�Q�b�g�r���[�̏�����
	immediate_context->OMSetRenderTargets(0, nullptr, nullptr);


	//�r���[�|�[�g�̍Đݒ�
	D3D11_VIEWPORT viewport{};

	viewport.TopLeftX = 0;									//��ʍ����x���W
	viewport.TopLeftY = 0;									//��ʍ����y���W
	viewport.Width = static_cast<float>(SCREEN_WIDTH);		//��ʂ̉��T�C�Y
	viewport.Height = static_cast<float>(SCREEN_HEIGHT);	//��ʂ̏c�T�C�Y
	viewport.MinDepth = 0.0f;								//�[�x�l�̍ŏ��l
	viewport.MaxDepth = 1.0f;								//�[�x�l�̍ő�l

	immediate_context->RSSetViewports(1, &viewport);		//�r���[�|�[�g��ݒ�


	//�V���h�E�}�b�v�e�N�X�`���̃o�C���h
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


