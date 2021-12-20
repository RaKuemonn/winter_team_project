
#include "sprite.h"
#include "misc.h"
#include <sstream>
#include "WICTextureLoader.h"

Sprite::Sprite(ID3D11Device* device, const char* filename)
{
    HRESULT hr{ S_OK };

    //���_���̃Z�b�g
    Vertex vertices[]
    {
        { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
        { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
        { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
        { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };


    //���_�o�b�t�@�I�u�W�F�N�g�̐���
    D3D11_BUFFER_DESC buffer_desc{};

    buffer_desc.ByteWidth = sizeof(vertices);               //�o�C�g�P�ʂ̃o�b�t�@�̃T�C�Y
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;                //�o�b�t�@�̓ǂݎ��Ə������݂̕��@(��ʓI�Ȓl��D3D11_USAGE_DEFAULT)
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       //�o�b�t�@���p�C�v���C���Ƀo�C���h�������@
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    //CPU�A�N�Z�X�t���O(0�Ȃ�s�v)
    buffer_desc.MiscFlags = 0;                              //���̑��̃t���O(0�Ȃ疢�g�p)
    buffer_desc.StructureByteStride = 0;                    //�o�b�t�@���\�����o�b�t�@��\���ꍇ�̃o�b�t�@�\�����̊e�v�f�̃T�C�Y

    D3D11_SUBRESOURCE_DATA subresource_data{};

    subresource_data.pSysMem = vertices;        //�������f�[�^�ւ̃|�C���^
    subresource_data.SysMemPitch = 0;           //�e�N�X�`����1�s�̐擪���玟�̍s�܂ł̋���
    subresource_data.SysMemSlicePitch = 0;      //����[�x���x���̊J�n���玟�̐[�x���x���܂ł̋���

    hr = device->CreateBuffer(
        &buffer_desc,           //�o�b�t�@�[���L�q����\���̂̃A�h���X
        &subresource_data,      //�������f�[�^���L�q����\���̂̃A�h���X
        &vertex_buffer          //�쐬���ꂽ�o�b�t�@�I�u�W�F�N�g�̃C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
    );

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    /*
    �����GPU(�ǂݎ��̂�)��CPU(�������݂̂�)�ɂ��A�N�Z�X���\�ɂ��Ă���
    */
    


    //���̓��C�A�E�g�I�u�W�F�N�g�̐���
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };


    //���_�V�F�[�_�[�I�u�W�F�N�g�̐���
    const char* cso_name_v{ "./CSO/sprite_vs.cso" };

    hr = create_vs_from_cso(device, cso_name_v, &vertex_shader, &input_layout, input_element_desc, _countof(input_element_desc));

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



    //�摜�t�@�C���̃��[�h�ƃV�F�[�_�[���\�[�X�r���[�I�u�W�F�N�g�̐���

    hr = load_texture_from_file(device, filename, &shader_resource_view, &texture2d_desc);

    release_all_textures();

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    //�s�N�Z���V�F�[�_�[�t�@�C���̃��[�h
    const char* cso_name_p{ "./CSO/sprite_ps.cso" };

    hr = create_ps_from_cso(device, cso_name_p, &pixel_shader);

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}


//�_(x, y)���_(cx, cy)�𒆐S�Ɋp(angle)�ŉ�]�������̍��W���v�Z����
inline void rotate(float& x, float& y, float cx, float cy, float& cos, float& sin)
{
    x -= cx;
    y -= cy;

    float tx{ x }, ty{ y };
    x = cos * tx + -sin * ty;
    y = sin * tx + cos * ty;

    x += cx;
    y += cy;
}


Vertex* Sprite::Screen_to_NDC(ID3D11DeviceContext* immediate_context,
    float dx, float dy, float dw, float dh,
    float sx, float sy, float sw, float sh,
    float px, float py,
    float r, float g, float b, float a,
    float angle/*degree*/)
{
    //�X�N���[��(�r���[�|�[�g)�̃T�C�Y���擾����
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);



    //��`�̊e���_�̈ʒu(�X�N���[�����W�n)���v�Z����
    //  (x0, y0) *----* (x1, y1) 
    //           |   /|
    //           |  / |
    //           | /  |
    //           |/   |
    //  (x2, y2) *----* (x3, y3) 

    DirectX::XMFLOAT2 size{ sw * dw, sh * dh };

    //left-top
    float x0{ dx - size.x * px };
    float y0{ dy - size.y * py };
    //right-top
    float x1{ dx + size.x * (1 - px) };
    float y1{ dy - size.y * py };
    //left-bottom
    float x2{ dx - size.x * px };
    float y2{ dy + size.y * (1 - py) };
    //right-bottom
    float x3{ dx + size.x * (1 - px) };
    float y3{ dy + size.y * (1 - py) };


    float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
    float sin{ sinf(DirectX::XMConvertToRadians(angle)) };


    //��]�̒��S����`�̒��S�_�ɂ����ꍇ
    float cx = dx;
    float cy = dy;
    rotate(x0, y0, cx, cy, cos, sin);
    rotate(x1, y1, cx, cy, cos, sin);
    rotate(x2, y2, cx, cy, cos, sin);
    rotate(x3, y3, cx, cy, cos, sin);



    //�X�N���[�����W�n����NDC�ւ̍��W�ϊ����s��
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;



    //�v�Z���ʂŒ��_�o�b�t�@�I�u�W�F�N�g���X�V����
    HRESULT hr{ S_OK };

    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};

    hr = immediate_context->Map(
        vertex_buffer,              //�C���^�[�t�F�[�X�ւ̃|�C���^
        0,                          //�T�u���\�[�X�̃C���f�b�N�X�ԍ�
        D3D11_MAP_WRITE_DISCARD,    //���\�[�X�ɑ΂���CPU�̓ǂݎ�肨��я������݌������w��
        0,                          //GPU���r�W�[�̂Ƃ���CPU���������邩���w��
        &mapped_subresource         //�}�b�v���ꂽ�T�u���\�[�X�̍\���̂̃A�h���X
    );

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

    Vertex* vertices{ reinterpret_cast<Vertex*>(mapped_subresource.pData) };
    if (vertices != nullptr)
    {
        vertices[0].position = { x0, y0, 0 };
        vertices[1].position = { x1, y1, 0 };
        vertices[2].position = { x2, y2, 0 };
        vertices[3].position = { x3, y3, 0 };
        vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r, g, b, a };

    }

    return vertices;
}


void Sprite::Bind(ID3D11DeviceContext* immediate_context)
{
    immediate_context->Unmap(vertex_buffer, 0);


    //���_�o�b�t�@�̃o�C���h
    UINT stride{ sizeof(Vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(
        0,                  //�o�C���f�B���O�p�̍ŏ��̓��̓X���b�g
        1,                  //�z����̒��_�o�b�t�@�̐�
        &vertex_buffer,     //���_�o�b�t�@�[�̔z��̃A�h���X
        &stride,            //�X�g���C�h�l�̔z��̃A�h���X
        &offset             //�I�t�Z�b�g�l�̔z��̃A�h���X
    );

    //�v���~�e�B�u�^�C�v�y�уf�[�^�̏����Ɋւ�����̃o�C���h
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
    immediate_context->IASetInputLayout(input_layout);

    //���X�^���C�U�[�X�e�[�g�̃o�C���h
    //immediate_context->RSSetState(rasterizer_state);

    //�V�F�[�_�[���\�[�X�̃o�C���h
    immediate_context->PSSetShaderResources(0, 1, &shader_resource_view);

    //�V�F�[�_�[�̃o�C���h
    immediate_context->VSSetShader(vertex_shader, nullptr, 0);
    immediate_context->PSSetShader(pixel_shader, nullptr, 0);

    //�v���~�e�B�u�̕`��
    immediate_context->Draw(4, 0);
}


void Sprite::render(ID3D11DeviceContext* immediate_context,
    float dx, float dy, float dw, float dh,
    float sx, float sy, float sw, float sh,
    float px, float py,
    float r, float g, float b, float a,
    float angle/*degree*/)
{
    Vertex* vertices = Screen_to_NDC(immediate_context, dx, dy, dw, dh, sx, sy, sw, sh, px, py, r, g, b, a, angle);

    if (vertices != nullptr)
    {
        vertices[0].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
        vertices[0].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
        vertices[1].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
        vertices[1].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
        vertices[2].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
        vertices[2].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
        vertices[3].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
        vertices[3].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
    }

    Bind(immediate_context);
}


Sprite::~Sprite()
{
    //COM�I�u�W�F�N�g�̉��
    vertex_shader->Release();
    pixel_shader->Release();
    input_layout->Release();
    vertex_buffer->Release();
    shader_resource_view->Release();
}