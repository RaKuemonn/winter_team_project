#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "shader.h"



class Shadow_Map : Shader
{
public:
    void initialize(ID3D11Device* device) override;


    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> shadow_depth_stencil_view;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shadow_shader_resource_view;

};
