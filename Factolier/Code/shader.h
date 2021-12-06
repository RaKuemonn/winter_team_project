#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "misc.h"


HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements);


HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader);


class Shader
{
protected:
    Microsoft::WRL::ComPtr<ID3D11VertexShader>      vertex_shader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>       pixel_shader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       input_layout;

    //シーン定数バッファ
    struct Scene_Constants
    {
        DirectX::XMFLOAT4X4 view_projection;	//ビュー・プロジェクション変換行列
        DirectX::XMFLOAT4 light_direction;		//ライトの向き
        DirectX::XMFLOAT4 camera_position;		//カメラ位置
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

public:
    virtual void initialize(ID3D11Device* device) = 0;

    virtual void begin(ID3D11DeviceContext* immediate_context) = 0;
    virtual void end(ID3D11DeviceContext* immediate_context) = 0;

    ID3D11VertexShader* get_vs() {return vertex_shader.Get();}
    ID3D11PixelShader* get_ps() { return pixel_shader.Get(); }
};
