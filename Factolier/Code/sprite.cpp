
#include "sprite.h"
#include "misc.h"
#include <sstream>
#include "WICTextureLoader.h"

Sprite::Sprite(ID3D11Device* device, const char* filename)
{
    HRESULT hr{ S_OK };

    //頂点情報のセット
    Vertex vertices[]
    {
        { { -1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 0, 0 } },
        { { +1.0, +1.0, 0 }, { 1, 1, 1, 1 }, { 1, 0 } },
        { { -1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 0, 1 } },
        { { +1.0, -1.0, 0 }, { 1, 1, 1, 1 }, { 1, 1 } },
    };


    //頂点バッファオブジェクトの生成
    D3D11_BUFFER_DESC buffer_desc{};

    buffer_desc.ByteWidth = sizeof(vertices);               //バイト単位のバッファのサイズ
    buffer_desc.Usage = D3D11_USAGE_DYNAMIC;                //バッファの読み取りと書き込みの方法(一般的な値はD3D11_USAGE_DEFAULT)
    buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;       //バッファがパイプラインにバインドされる方法
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    //CPUアクセスフラグ(0なら不要)
    buffer_desc.MiscFlags = 0;                              //その他のフラグ(0なら未使用)
    buffer_desc.StructureByteStride = 0;                    //バッファが構造化バッファを表す場合のバッファ構造内の各要素のサイズ

    D3D11_SUBRESOURCE_DATA subresource_data{};

    subresource_data.pSysMem = vertices;        //初期化データへのポインタ
    subresource_data.SysMemPitch = 0;           //テクスチャの1行の先頭から次の行までの距離
    subresource_data.SysMemSlicePitch = 0;      //ある深度レベルの開始から次の深度レベルまでの距離

    hr = device->CreateBuffer(
        &buffer_desc,           //バッファーを記述する構造体のアドレス
        &subresource_data,      //初期化データを記述する構造体のアドレス
        &vertex_buffer          //作成されたバッファオブジェクトのインターフェイスへのポインタのアドレス
    );

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    /*
    今回はGPU(読み取りのみ)とCPU(書き込みのみ)によるアクセスを可能にしている
    */
    


    //入力レイアウトオブジェクトの生成
    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
        D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };


    //頂点シェーダーオブジェクトの生成
    const char* cso_name_v{ "./CSO/sprite_vs.cso" };

    hr = create_vs_from_cso(device, cso_name_v, &vertex_shader, &input_layout, input_element_desc, _countof(input_element_desc));

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



    //画像ファイルのロードとシェーダーリソースビューオブジェクトの生成

    hr = load_texture_from_file(device, filename, &shader_resource_view, &texture2d_desc);

    release_all_textures();

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    //ピクセルシェーダーファイルのロード
    const char* cso_name_p{ "./CSO/sprite_ps.cso" };

    hr = create_ps_from_cso(device, cso_name_p, &pixel_shader);

    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}


//点(x, y)が点(cx, cy)を中心に角(angle)で回転した時の座標を計算する
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
    //スクリーン(ビューポート)のサイズを取得する
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    immediate_context->RSGetViewports(&num_viewports, &viewport);



    //矩形の各頂点の位置(スクリーン座標系)を計算する
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


    //回転の中心を矩形の中心点にした場合
    float cx = dx;
    float cy = dy;
    rotate(x0, y0, cx, cy, cos, sin);
    rotate(x1, y1, cx, cy, cos, sin);
    rotate(x2, y2, cx, cy, cos, sin);
    rotate(x3, y3, cx, cy, cos, sin);



    //スクリーン座標系からNDCへの座標変換を行う
    x0 = 2.0f * x0 / viewport.Width - 1.0f;
    y0 = 1.0f - 2.0f * y0 / viewport.Height;
    x1 = 2.0f * x1 / viewport.Width - 1.0f;
    y1 = 1.0f - 2.0f * y1 / viewport.Height;
    x2 = 2.0f * x2 / viewport.Width - 1.0f;
    y2 = 1.0f - 2.0f * y2 / viewport.Height;
    x3 = 2.0f * x3 / viewport.Width - 1.0f;
    y3 = 1.0f - 2.0f * y3 / viewport.Height;



    //計算結果で頂点バッファオブジェクトを更新する
    HRESULT hr{ S_OK };

    D3D11_MAPPED_SUBRESOURCE mapped_subresource{};

    hr = immediate_context->Map(
        vertex_buffer,              //インターフェースへのポインタ
        0,                          //サブリソースのインデックス番号
        D3D11_MAP_WRITE_DISCARD,    //リソースに対するCPUの読み取りおよび書き込み権限を指定
        0,                          //GPUがビジーのときにCPUが何をするかを指定
        &mapped_subresource         //マップされたサブリソースの構造体のアドレス
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


    //頂点バッファのバインド
    UINT stride{ sizeof(Vertex) };
    UINT offset{ 0 };
    immediate_context->IASetVertexBuffers(
        0,                  //バインディング用の最初の入力スロット
        1,                  //配列内の頂点バッファの数
        &vertex_buffer,     //頂点バッファーの配列のアドレス
        &stride,            //ストライド値の配列のアドレス
        &offset             //オフセット値の配列のアドレス
    );

    //プリミティブタイプ及びデータの順序に関する情報のバインド
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    //入力レイアウトオブジェクトのバインド
    immediate_context->IASetInputLayout(input_layout);

    //ラスタライザーステートのバインド
    //immediate_context->RSSetState(rasterizer_state);

    //シェーダーリソースのバインド
    immediate_context->PSSetShaderResources(0, 1, &shader_resource_view);

    //シェーダーのバインド
    immediate_context->VSSetShader(vertex_shader, nullptr, 0);
    immediate_context->PSSetShader(pixel_shader, nullptr, 0);

    //プリミティブの描画
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
    //COMオブジェクトの解放
    vertex_shader->Release();
    pixel_shader->Release();
    input_layout->Release();
    vertex_buffer->Release();
    shader_resource_view->Release();
}