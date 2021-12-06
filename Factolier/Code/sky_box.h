#pragma once

#include <d3d11.h>
#include <wrl.h>



class Sky_Box
{
public:
    Sky_Box(ID3D11Device* device, const wchar_t* filename);
    ~Sky_Box() {}

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    shader_resource_view;
};
