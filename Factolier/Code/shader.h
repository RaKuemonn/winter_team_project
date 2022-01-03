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

    //シーン定数バッファ
    struct Scene_Constant
    {
        DirectX::XMFLOAT4X4 view_projection {};	    //ビュー・プロジェクション変換行列
        DirectX::XMFLOAT4X4 view_rotate {};         //ビューの回転行列
        DirectX::XMFLOAT4 camera_position {};		//カメラ位置
        DirectX::XMFLOAT4 timer {};                 //経過時間
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> scene_buffer;

    //ライト定数バッファ
    struct Light_Constant
    {
        DirectX::XMFLOAT4 ambient_color{ 0.2f, 0.2f, 0.2f, 0.2f };      //アンビエントカラー
        DirectX::XMFLOAT4 light_direction{ 0.0f, -1.0f, 0.0f, 1.0f };   //ライトの向き
        DirectX::XMFLOAT4 light_radiance{ 1.0f, 1.0f, 1.0f, 1.0f };     //ライトの輝度
        DirectX::XMFLOAT4 light_color{ 1.0f, 1.0f, 1.0f, 1.0f };        //ライトの色
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> light_buffer;

    //フォグ定数バッファ
    struct Fog_Constant
    {
        DirectX::XMFLOAT4 fog_color{ 1.0f, 1.0f, 1.0f, 1.0f };          //フォグの色
        DirectX::XMFLOAT4 fog_range{ 0.0f, 100.0f, 0.0f, 0.0f };        //フォグの距離(x = near, y = far)
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> fog_buffer;

    //シャドウ定数バッファ
    struct Shadow_Constant
    {
        DirectX::XMFLOAT4X4 light_view_projection{};	                        // ライトの位置から見た射影行列
        DirectX::XMFLOAT3	shadow_color{ 0.3f, 0.3f, 0.3f };			// 影色
        float				shadow_bias{ 0.008f };			                    // 深度バイアス
    };
    Microsoft::WRL::ComPtr<ID3D11Buffer> shadow_buffer;



public:
    virtual void initialize(ID3D11Device* device) = 0;

    virtual void begin(ID3D11DeviceContext* immediate_context, bool is_shadow_map = false, float elapsed_time = 0.0f) = 0;
    virtual void end(ID3D11DeviceContext* immediate_context) = 0;

    ID3D11VertexShader* get_vs() {return vertex_shader.Get();}
    ID3D11PixelShader* get_ps() { return pixel_shader.Get(); }
};
