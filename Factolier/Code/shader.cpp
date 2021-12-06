
#include <WICTextureLoader.h>

#include <sstream>
#include <memory>

#include "shader.h"


//頂点シェーダーオブジェクトの生成
HRESULT create_vs_from_cso(ID3D11Device* device, const char* cso_name, ID3D11VertexShader** vertex_shader,
    ID3D11InputLayout** input_layout, D3D11_INPUT_ELEMENT_DESC* input_element_desc, UINT num_elements)
{
    using namespace std;

    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);     //ファイル位置指示子のセット
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    unique_ptr<unsigned char[]> cso_data{ make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);


    HRESULT hr{ S_OK };

    hr = device->CreateVertexShader(
        cso_data.get(),     //コンパイルされたシェーダーへのポインタ
        cso_sz,             //コンパイルされた頂点シェーダーのサイズ
        nullptr,            //インターフェースへのポインターのアドレス(nullの場合全てのパラメータが検証される)
        vertex_shader       //インターフェースへのポインターのアドレス
    );

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    if (input_layout)
    {
        hr = device->CreateInputLayout(
            input_element_desc,     //入力アセンブラステージの入力データ型の配列
            num_elements,           //input要素の配列内の入力データ型の数
            cso_data.get(),         //コンパイルされたシェーダーへのポインタ
            cso_sz,                 //コンパイルされたシェーダーのサイズ
            input_layout            //作成されたinput-layoutオブジェクトのアドレス
        );

        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    return hr;
}


//ピクセルシェーダーオブジェクトの生成
HRESULT create_ps_from_cso(ID3D11Device* device, const char* cso_name, ID3D11PixelShader** pixel_shader)
{
    using namespace std;

    FILE* fp{ nullptr };
    fopen_s(&fp, cso_name, "rb");
    _ASSERT_EXPR_A(fp, "CSO File not found");

    fseek(fp, 0, SEEK_END);     //ファイル位置指示子のセット
    long cso_sz{ ftell(fp) };
    fseek(fp, 0, SEEK_SET);

    unique_ptr<unsigned char[]> cso_data{ make_unique<unsigned char[]>(cso_sz) };
    fread(cso_data.get(), cso_sz, 1, fp);
    fclose(fp);


    HRESULT hr{ S_OK };

    hr = device->CreatePixelShader(
        cso_data.get(),     //コンパイルされたシェーダーへのポインタ
        cso_sz,             //コンパイルされたピクセルシェーダーのサイズ
        nullptr,            //インターフェースへのポインターのアドレス(nullの場合全てのパラメータが検証される)
        pixel_shader        //インターフェースへのポインターのアドレス
    );

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    return hr;
}