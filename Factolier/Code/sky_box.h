#pragma once

#include <d3d11.h>
#include <wrl.h>



class Sky_Box
{
public:
    Sky_Box(ID3D11Device* device, const wchar_t* filename);
    ~Sky_Box() {}

    void render(ID3D11DeviceContext* immediate_context);


    struct Vertex
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT3 normal;
    };

    void create_com_buffers(ID3D11Device* device, Vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count);

private:
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    shader_resource_view;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
};
