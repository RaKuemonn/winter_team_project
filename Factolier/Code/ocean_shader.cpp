
#include "ocean_shader.h"
#include "camera.h"


void Ocean_Shader::initialize(ID3D11Device* device)
{
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    //シェーダーの生成
    create_vs_from_cso(device, "./CSO/ocean_vs.cso", vertex_shader.ReleaseAndGetAddressOf(), input_layout.ReleaseAndGetAddressOf(),
        input_element_desc, ARRAYSIZE(input_element_desc));
    create_ps_from_cso(device, "./CSO/ocean_ps.cso", pixel_shader.ReleaseAndGetAddressOf());


    HRESULT hr{ S_OK };
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


    //ライトバッファ
    buffer_desc.ByteWidth = sizeof(Light_Constant);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;

    hr = device->CreateBuffer(&buffer_desc, nullptr, light_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    //フォグバッファ
    buffer_desc.ByteWidth = sizeof(Fog_Constant);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;

    hr = device->CreateBuffer(&buffer_desc, nullptr, fog_buffer.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


void Ocean_Shader::begin(ID3D11DeviceContext* immediate_context, float elapsed_time)
{
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    immediate_context->IASetInputLayout(input_layout.Get());

    immediate_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
    immediate_context->PSSetShader(pixel_shader.Get(), nullptr, 0);


    scroll_timer += elapsed_time;
    if (scroll_timer >= FLT_MAX - 10.0f) scroll_timer = 0.0f;


    Camera& camera = Camera::Instance();

    Scene_Constant scene_constant{};
    DirectX::XMStoreFloat4x4(&scene_constant.view_projection, DirectX::XMLoadFloat4x4(&camera.get_view()) * DirectX::XMLoadFloat4x4(&camera.get_projection()));
    scene_constant.camera_position = { 0, 0, 0, 0 };
    scene_constant.timer.x = scroll_timer;
    immediate_context->UpdateSubresource(scene_buffer.Get(), 0, 0, &scene_constant, 0, 0);
    immediate_context->VSSetConstantBuffers(1, 1, scene_buffer.GetAddressOf());
    immediate_context->PSSetConstantBuffers(1, 1, scene_buffer.GetAddressOf());

    Light_Constant light_constant{};
    light_constant.light_direction = { camera.get_front().x, camera.get_front().y, camera.get_front().z, 0 };
    immediate_context->UpdateSubresource(light_buffer.Get(), 0, 0, &light_constant, 0, 0);
    immediate_context->VSSetConstantBuffers(2, 1, light_buffer.GetAddressOf());
    immediate_context->PSSetConstantBuffers(2, 1, light_buffer.GetAddressOf());

    Fog_Constant fog_constant{};
    //fog_constant.fog_range = { camera.get_front().x, camera.get_front().y, camera.get_front().z, 0 };
    immediate_context->UpdateSubresource(fog_buffer.Get(), 0, 0, &fog_constant, 0, 0);
    immediate_context->VSSetConstantBuffers(3, 1, fog_buffer.GetAddressOf());
    immediate_context->PSSetConstantBuffers(3, 1, fog_buffer.GetAddressOf());

}


void Ocean_Shader::end(ID3D11DeviceContext* immediate_context)
{

}