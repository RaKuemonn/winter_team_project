#pragma once

#include <vector>
#include "shader.h"


class Shader_Manager
{
public:
    Shader_Manager(ID3D11Device* device) { initialize(device); }
    ~Shader_Manager() { uninitialize(); }

    void initialize(ID3D11Device* device);
    void uninitialize();

    Shader* get_shader(int num) { return shaders[num]; }

private:
    std::vector<Shader*> shaders;
};