
#include <DirectXTex.h>
#include "misc.h"
#include "sky_box.h"



Sky_Box::Sky_Box(ID3D11Device* device, const wchar_t* filename)
{
    using namespace DirectX;


    HRESULT hr{ S_OK };

    TexMetadata tex_metadata;
    ScratchImage scratch_image;

    hr = LoadFromDDSFile(filename, DDS_FLAGS_NONE, &tex_metadata, scratch_image);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    D3D11_TEXTURE2D_DESC desc;
    desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_TEXTURECUBE;
    desc.ArraySize = static_cast<UINT>(tex_metadata.arraySize);
    desc.MipLevels = static_cast<UINT>(tex_metadata.mipLevels);

    std::vector<D3D11_SUBRESOURCE_DATA> initData;
    auto image = scratch_image.GetImages();
    for (int i = 0; i < tex_metadata.arraySize; ++i)
    {
        for (int j = 0; j < tex_metadata.mipLevels; ++j)
        {
            int index = i * static_cast<int>(tex_metadata.mipLevels) + j;
            D3D11_SUBRESOURCE_DATA data;
            data.pSysMem = image[index].pixels;
            data.SysMemPitch = static_cast<UINT>(image[index].rowPitch);
            data.SysMemSlicePitch = 0;
            initData.push_back(data);
        }
    }



    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = static_cast<UINT>(tex_metadata.width);
    texture2d_desc.Height = static_cast<UINT>(tex_metadata.height);
    texture2d_desc.MipLevels = static_cast<UINT>(tex_metadata.mipLevels);
    texture2d_desc.ArraySize = static_cast<UINT>(tex_metadata.arraySize);
    texture2d_desc.Format = tex_metadata.format;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texture2d_desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_TEXTURECUBE;


    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;

    //テクスチャの生成
    hr = device->CreateTexture2D(&texture2d_desc, initData.data(), &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    shader_resource_view_desc.TextureCube.MipLevels = static_cast<UINT>(tex_metadata.mipLevels);
    shader_resource_view_desc.TextureCube.MostDetailedMip = 0;

    //シェーダーリソースビューの生成
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc, shader_resource_view.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    //頂点の生成
    Vertex vertices[24]{};

    vertices[0].position = { -0.5f, 0.5f, 0.5f };
    vertices[0].normal = { 0.0f, 1.0f, 0.0f };
    vertices[1].position = { 0.5f, 0.5f, 0.5f };
    vertices[1].normal = { 0.0f, 1.0f, 0.0f };
    vertices[2].position = { -0.5f, 0.5f, -0.5f };
    vertices[2].normal = { 0.0f, 1.0f, 0.0f };
    vertices[3].position = { 0.5f, 0.5f, -0.5f };
    vertices[3].normal = { 0.0f, 1.0f, 0.0f };

    vertices[4].position = { -0.5f, 0.5f, -0.5f };
    vertices[4].normal = { 0.0f, 0.0f, -1.0f };
    vertices[5].position = { 0.5f, 0.5f, -0.5f };
    vertices[5].normal = { 0.0f, 0.0f, -1.0f };
    vertices[6].position = { -0.5f, -0.5f, -0.5f };
    vertices[6].normal = { 0.0f, 0.0f, -1.0f };
    vertices[7].position = { 0.5f, -0.5f, -0.5f };
    vertices[7].normal = { 0.0f, 0.0f, -1.0f };

    vertices[8].position = { 0.5f, 0.5f, -0.5f };
    vertices[8].normal = { 1.0f, 0.0f, 0.0f };
    vertices[9].position = { 0.5f, 0.5f, 0.5f };
    vertices[9].normal = { 1.0f, 0.0f, 0.0f };
    vertices[10].position = { 0.5f, -0.5f, -0.5f };
    vertices[10].normal = { 1.0f, 0.0f, 0.0f };
    vertices[11].position = { 0.5f, -0.5f, 0.5f };
    vertices[11].normal = { 1.0f, 0.0f, 0.0f };

    vertices[12].position = { 0.5f, 0.5f, 0.5f };
    vertices[12].normal = { 0.0f, 0.0f, 1.0f };
    vertices[13].position = { -0.5f, 0.5f, 0.5f };
    vertices[13].normal = { 0.0f, 0.0f, 1.0f };
    vertices[14].position = { 0.5f, -0.5f, 0.5f };
    vertices[14].normal = { 0.0f, 0.0f, 1.0f };
    vertices[15].position = { -0.5f, -0.5f, 0.5f };
    vertices[15].normal = { 0.0f, 0.0f, 1.0f };

    vertices[16].position = { -0.5f, 0.5f, 0.5f };
    vertices[16].normal = { -1.0f, 0.0f, 0.0f };
    vertices[17].position = { -0.5f, 0.5f, -0.5f };
    vertices[17].normal = { -1.0f, 0.0f, 0.0f };
    vertices[18].position = { -0.5f, -0.5f, 0.5f };
    vertices[18].normal = { -1.0f, 0.0f, 0.0f };
    vertices[19].position = { -0.5f, -0.5f, -0.5f };
    vertices[19].normal = { -1.0f, 0.0f, 0.0f };

    vertices[20].position = { -0.5f, -0.5f, -0.5f };
    vertices[20].normal = { 0.0f, 0.0f, -1.0f };
    vertices[21].position = { 0.5f, -0.5f, -0.5f };
    vertices[21].normal = { 0.0f, 0.0f, -1.0f };
    vertices[22].position = { -0.5f, -0.5f, 0.5f };
    vertices[22].normal = { 0.0f, 0.0f, -1.0f };
    vertices[23].position = { 0.5f, -0.5f, 0.5f };
    vertices[23].normal = { 0.0f, 0.0f, -1.0f };


    uint32_t indices[36]{};

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    indices[3] = 1;
    indices[4] = 3;
    indices[5] = 2;

    indices[6] = 4;
    indices[7] = 5;
    indices[8] = 6;

    indices[9] = 5;
    indices[10] = 7;
    indices[11] = 6;

    indices[12] = 8;
    indices[13] = 9;
    indices[14] = 10;

    indices[15] = 9;
    indices[16] = 11;
    indices[17] = 10;

    indices[18] = 12;
    indices[19] = 13;
    indices[20] = 14;

    indices[21] = 13;
    indices[22] = 15;
    indices[23] = 14;

    indices[24] = 16;
    indices[25] = 17;
    indices[26] = 18;

    indices[27] = 17;
    indices[28] = 19;
    indices[29] = 18;

    indices[30] = 20;
    indices[31] = 21;
    indices[32] = 22;

    indices[33] = 21;
    indices[34] = 23;
    indices[35] = 22;

    //バッファの生成
    create_com_buffers(device, vertices, 24, indices, 36);
}


void Sky_Box::render(ID3D11DeviceContext* immediate_context)
{
    uint32_t stride{ sizeof(Vertex) };
    uint32_t offset{ 0 };

    immediate_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    immediate_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());

    D3D11_BUFFER_DESC buffer_desc{};
    index_buffer->GetDesc(&buffer_desc);
    immediate_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
}


void Sky_Box::create_com_buffers(ID3D11Device* device, Vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
    HRESULT hr{ S_OK };

    D3D11_BUFFER_DESC buffer_desc{};
    D3D11_SUBRESOURCE_DATA subresource_data{};
    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertex_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags = 0;
    buffer_desc.StructureByteStride = 0;
    subresource_data.pSysMem = vertices;
    subresource_data.SysMemPitch = 0;
    subresource_data.SysMemSlicePitch = 0;

    hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    subresource_data.pSysMem = indices;

    hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


