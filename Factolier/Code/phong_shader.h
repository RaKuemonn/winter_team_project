#pragma once

#include "shader.h"


class Phong_Shader : public Shader
{
public:
    void initialize(ID3D11Device* device) override;

    void begin(ID3D11DeviceContext* immediate_context, bool is_shadow_map = false, float elapsed_time = 0.0f) override;
    void end(ID3D11DeviceContext* immediate_context) override;
};
