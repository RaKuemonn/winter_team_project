#pragma once

#include <DirectXMath.h>


class Light
{
public:
    Light() {}


    void set_ambient(DirectX::XMFLOAT4 ambient) { ambient_color = ambient; }
    void set_direction(DirectX::XMFLOAT4 direction) { light_direction = direction; }
    void set_position(DirectX::XMFLOAT4 position) { light_position = position; }
    void set_radiance(DirectX::XMFLOAT4 radiance) { light_radiance = radiance; }
    void set_color(DirectX::XMFLOAT4 color) { light_color = color; }


    DirectX::XMFLOAT4 ambient_color{ 0.2f, 0.2f, 0.2f, 0.2f };      //アンビエントカラー
    DirectX::XMFLOAT4 light_direction{ -1.0f, -1.0f, -1.0f, 1.0f };   //ライトの向き
    DirectX::XMFLOAT4 light_position{ 60.0f, 60.0f, 60.0f, 0.0f };    //ライトの位置
    DirectX::XMFLOAT4 light_radiance{ 1.0f, 1.0f, 1.0f, 1.0f };     //ライトの輝度
    DirectX::XMFLOAT4 light_color{ 1.0f, 1.0f, 1.0f, 1.0f };        //ライトの色
};
