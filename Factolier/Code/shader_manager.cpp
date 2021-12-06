
#include "shader_manager.h"
#include "skinned_shader.h"


void Shader_Manager::initialize(ID3D11Device* device)
{
    shaders.emplace_back(new Skinned_Shader());

    for (Shader* s : shaders)
    {
        s->initialize(device);
    }
}


void Shader_Manager::uninitialize()
{
    for (Shader* s : shaders)
    {
        delete s;
        s = nullptr;
    }

    shaders.clear();
}

