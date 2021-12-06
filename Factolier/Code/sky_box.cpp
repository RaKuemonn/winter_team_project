
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
}
