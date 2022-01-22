#pragma once

#include "transform.h"
#include "model.h"

// Debug‚Ì‚İ“®ì
class Decotator_Supporter final
{
public:
    Decotator_Supporter(class Scene_Manager* ptr_scene_manager_);
    ~Decotator_Supporter() = default;

public:
    void imgui_control();
    void render();

private:
    std::unique_ptr<Transform>  m_transform;
    std::unique_ptr<Model>      m_model;

};

