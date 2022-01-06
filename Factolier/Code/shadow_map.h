#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "shader.h"



class Shadow_Map : public Shader
{
public:
    ~Shadow_Map() override {}

    void initialize(ID3D11Device* device) override;

    void begin(ID3D11DeviceContext* immediate_context, float elapsed_time = 0.0f) override;
    void end(ID3D11DeviceContext* immediate_context) override;


    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> shadow_depth_stencil_view;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shadow_shader_resource_view;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> shadow_sampler_state;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> org_depth_stencil_view;

};
