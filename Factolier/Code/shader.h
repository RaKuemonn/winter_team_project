#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "misc.h"
#include "light_manager.h"


HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);


HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);


class Shader
{
protected:
    Microsoft::WRL::ComPtr<ID3D11VertexShader>      vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       input_layout;

    //�V�[���萔�o�b�t�@
    struct Scene_Constant
    {
        DirectX::XMFLOAT4X4 view_projection {};	    //�r���[�E�v���W�F�N�V�����ϊ��s��
        DirectX::XMFLOAT4X4 view_rotate {};         //�r���[�̉�]�s��
        DirectX::XMFLOAT4 camera_position {};		//�J�����ʒu
        DirectX::XMFLOAT4 timer {};                 //�o�ߎ���
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> scene_buffer;

    //���C�g�萔�o�b�t�@
    struct Light_Constant
    {
        DirectX::XMFLOAT4 ambient_color{ 0.2f, 0.2f, 0.2f, 0.2f };      //�A���r�G���g�J���[
        DirectX::XMFLOAT4 light_direction{ 0.0f, -1.0f, 0.0f, 1.0f };   //���C�g�̌���
        DirectX::XMFLOAT4 light_radiance{ 1.0f, 1.0f, 1.0f, 1.0f };     //���C�g�̋P�x
        DirectX::XMFLOAT4 light_color{ 1.0f, 1.0f, 1.0f, 1.0f };        //���C�g�̐F
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> light_buffer;

    //�t�H�O�萔�o�b�t�@
    struct Fog_Constant
    {
        DirectX::XMFLOAT4 fog_color{ 1.0f, 1.0f, 1.0f, 1.0f };          //�t�H�O�̐F
        DirectX::XMFLOAT4 fog_range{ 0.0f, 100.0f, 0.0f, 0.0f };        //�t�H�O�̋���(x = near, y = far)
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> fog_buffer;

    //�V���h�E�萔�o�b�t�@
    struct Shadow_Constant
    {
        DirectX::XMFLOAT4X4 light_view_projection{};	                        // ���C�g�̈ʒu���猩���ˉe�s��
        DirectX::XMFLOAT3	shadow_color{ 0.3f, 0.3f, 0.3f };			// �e�F
        float				shadow_bias{ 0.008f };			                    // �[�x�o�C�A�X
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> shadow_buffer;



public:
    virtual void initialize(ID3D11Device* device) = 0;

    virtual void begin(ID3D11DeviceContext* immediate_context, bool is_shadow_map = false, float elapsed_time = 0.0f) = 0;
    virtual void end(ID3D11DeviceContext* immediate_context) = 0;

    ID3D11VertexShader* get_vs() {return vertex_shader.Get();}
    ID3D11PixelShader* get_ps() { return pixel_shader.Get(); }
};
