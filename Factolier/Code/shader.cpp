
#include <WICTextureLoader.h>

#include <sstream>
#include <memory>

#include "shader.h"


//���_�V�F�[�_�[�I�u�W�F�N�g�̐���
HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    using namespace std;

    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);     //�t�@�C���ʒu�w���q�̃Z�b�g
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    unique_ptr<unsigned char[]> cso_data{ make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);


    HRESULT hr{ S_OK };

    hr = device->CreateVertexShader(
        cso_data.get(),     //�R���p�C�����ꂽ�V�F�[�_�[�ւ̃|�C���^
        cso_sz,             //�R���p�C�����ꂽ���_�V�F�[�_�[�̃T�C�Y
        nullptr,            //�C���^�[�t�F�[�X�ւ̃|�C���^�[�̃A�h���X(null�̏ꍇ�S�Ẵp�����[�^�����؂����)
        vertex_shader       //�C���^�[�t�F�[�X�ւ̃|�C���^�[�̃A�h���X
    );

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    if (input_layout)
    {
        hr = device->CreateInputLayout(
            input_element_desc,     //���̓A�Z���u���X�e�[�W�̓��̓f�[�^�^�̔z��
            num_elements,           //input�v�f�̔z����̓��̓f�[�^�^�̐�
            cso_data.get(),         //�R���p�C�����ꂽ�V�F�[�_�[�ւ̃|�C���^
            cso_sz,                 //�R���p�C�����ꂽ�V�F�[�_�[�̃T�C�Y
            input_layout            //�쐬���ꂽinput-layout�I�u�W�F�N�g�̃A�h���X
        );

        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    return hr;
}


//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̐���
HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    using namespace std;

    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);     //�t�@�C���ʒu�w���q�̃Z�b�g
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    unique_ptr<unsigned char[]> cso_data{ make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);


    HRESULT hr{ S_OK };

    hr = device->CreatePixelShader(
        cso_data.get(),     //�R���p�C�����ꂽ�V�F�[�_�[�ւ̃|�C���^
        cso_sz,             //�R���p�C�����ꂽ�s�N�Z���V�F�[�_�[�̃T�C�Y
        nullptr,            //�C���^�[�t�F�[�X�ւ̃|�C���^�[�̃A�h���X(null�̏ꍇ�S�Ẵp�����[�^�����؂����)
        pixel_shader        //�C���^�[�t�F�[�X�ւ̃|�C���^�[�̃A�h���X
    );

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    return hr;
}