#pragma once

#include <vector>
#include "shader.h"


enum class Shaders
{
    PHONG = 0,
    SKY,
    OCEAN,
};


class Shader_Manager
{
public:
    Shader_Manager(ID3D11Device* device) { initialize(device); }
    ~Shader_Manager() { uninitialize(); }

    void initialize(ID3D11Device* device);
    void uninitialize();

    Shader* get_shader(Shaders num) { return shaders[static_cast<int>(num)]; }

private:
    std::vector<Shader*> shaders;
};