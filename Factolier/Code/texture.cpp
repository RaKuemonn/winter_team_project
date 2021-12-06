
#include "texture.h"
#include <filesystem>
#include <DDSTextureLoader.h>


//�摜�t�@�C���̃��[�h�ƃV�F�[�_�[���\�[�X�r���[�I�u�W�F�N�g�̐���
static map<wstring, ComPtr<ID3D11ShaderResourceView>> resources;

HRESULT load_texture_from_file(ID3D11Device* device, const char* filename,
    ID3D11ShaderResourceView** shader_resource_view, D3D11_TEXTURE2D_DESC* texture2d_desc)
{
    HRESULT hr{ S_OK };
    ComPtr<ID3D11Resource> resource;

    wchar_t _fileName[256];
    MultiByteToWideChar(CP_ACP, 0, filename, static_cast<int>(strlen(filename) + 1), _fileName, 256);
    

    auto it = resources.find(_fileName);
    if (it != resources.end())
    {
        *shader_resource_view = it->second.Get();
        (*shader_resource_view)->AddRef();
        (*shader_resource_view)->GetResource(resource.GetAddressOf());

    }

    else
    {
        std::filesystem::path dds_filename(filename);
        dds_filename.replace_extension("dds");
        if (std::filesystem::exists(dds_filename.c_str()))
        {
            Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context;
            device->GetImmediateContext(immediate_context.GetAddressOf());
            hr = DirectX::CreateDDSTextureFromFile(device, immediate_context.Get(), dds_filename.c_str(),
                resource.GetAddressOf(), shader_resource_view);
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
        }

        else
        {
            hr = CreateWICTextureFromFile(
                device,                     //�e�N�X�`���Ɋ֘A�t����f�o�C�X�̃|�C���^
                _fileName,                  //�t�@�C�������w�肷�镶����̃|�C���^
                resource.GetAddressOf(),    //���\�[�X�̃A�h���X
                shader_resource_view        //�쐬���ꂽshader_resource_view�I�u�W�F�N�g�̃A�h���X
            );
            _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

            resources.insert(make_pair(_fileName, *shader_resource_view));
        }
    }

    //�e�N�X�`�����̎擾
    ComPtr<ID3D11Texture2D> texture2d;

    hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    texture2d->GetDesc(texture2d_desc);     //�f�X�N�̎擾


    return hr;
}


void release_all_textures()
{
    resources.clear();
}