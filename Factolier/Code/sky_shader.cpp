
#include "sky_shader.h"
#include "camera.h"


void Sky_Shader::initialize(ID3D11Device* device)
{
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    //シェーダーの生成
    create_vs_from_cso(device, "./CSO/sky_box_vs.cso", vertex_shader.ReleaseAndGetAddressOf(), input_layout.ReleaseAndGetAddressOf(),
        input_element_desc, ARRAYSIZE(input_element_desc));
    create_ps_from_cso(device, "./CSO/sky_box_ps.cso", pixel_shader.ReleaseAndGetAddressOf());


    HRESULT hr{ S_OK };
    //定数バッファオブジェクトの生成

    D3D11_BUFFER_DESC buffer_desc{};
    buffer_desc.ByteWidth = sizeof(Scene_Constant);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;

    hr = device->CreateBuffer(&buffer_desc, nullptr, scene_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


void Sky_Shader::begin(ID3D11DeviceContext* immediate_context, bool is_shadow_map, float elapsed_time)
{
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());

    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

    Scene_Constant data{};
    Camera& camera = Camera::Instance();
    DirectX::XMFLOAT3 eye = { 0.0f, 0.0f, 0.0f };
    DirectX::XMVECTOR eye_v = DirectX::XMLoadFloat3(&eye);
    DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
    DirectX::XMVECTOR up_v = DirectX::XMLoadFloat3(&up);

    DirectX::XMMATRIX rotate = DirectX::XMMatrixLookAtLH(eye_v, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&camera.get_front())), up_v);

    DirectX::XMStoreFloat4x4(&data.view_rotate, rotate * DirectX::XMLoadFloat4x4(&camera.get_projection()));
    //DirectX::XMStoreFloat4x4(&data.view_rotate, DirectX::XMLoadFloat4x4(&Camera::Instance().get_projection()));
    DirectX::XMStoreFloat4x4(&data.view_projection, DirectX::XMLoadFloat4x4(&camera.get_view()) * DirectX::XMLoadFloat4x4(&camera.get_projection()));

    immediate_context->UpdateSubresource(scene_buffer.Get(), 0, 0, &data, 0, 0);
    immediate_context->VSSetConstantBuffers(1, 1, scene_buffer.GetAddressOf());
    immediate_context->PSSetConstantBuffers(1, 1, scene_buffer.GetAddressOf());
}


void Sky_Shader::end(ID3D11DeviceContext* immediate_context)
{
    
}

