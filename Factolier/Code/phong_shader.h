#pragma once

#include "shader.h"


class Phong_Shader : public Shader
{
public:
    ~Phong_Shader() override {}

    void initialize(ID3D11Device* device) override;

    void begin(ID3D11DeviceContext* immediate_context, float elapsed_time = 0.0f) override;
    void end(ID3D11DeviceContext* immediate_context) override;
};
