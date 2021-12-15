
#include "shader_manager.h"
#include "phong_shader.h"
#include "sky_shader.h"
#include "ocean_shader.h"


void Shader_Manager::initialize(ID3D11Device* device)
{
    shaders.emplace_back(new Phong_Shader());
    shaders.emplace_back(new Sky_Shader());
    shaders.emplace_back(new Ocean_Shader());

    for (Shader* s : shaders)
    {
        s->initialize(device);
    }
}


void Shader_Manager::uninitialize()
{
    for (Shader* s : shaders)
    {
        safe_delete(s);
    }

    shaders.clear();
}

