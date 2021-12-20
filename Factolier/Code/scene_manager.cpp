
#include "scene_manager.h"


void Scene_Manager::initialize(ID3D11Device* device, ID3D11DeviceContext* context, HRESULT hr)
{
    device_ = device;
    immediate_context_ = context;

    state_manager_  = std::make_unique<State_Manager>(device, context, hr);
    shader_manager_ = std::make_unique<Shader_Manager>(device);
    model_manager_  = std::make_unique<Model_Manager>(device);
    sound_manager_  = std::make_unique<Sound_Manager>();
    input_manager_  = std::make_unique<Input_Manager_Interface>();
}


void Scene_Manager::uninitialize()
{
    clear();
}


void Scene_Manager::update(float elapsed_time, const HWND& hwnd_)
{
    input_manager_.get()->update(hwnd_);

    if (scene != nullptr)
    {
        scene->update(elapsed_time);
    }

    sound_manager_.get()->update();
}


void Scene_Manager::render(float elapsed_time)
{
    if (scene != nullptr)
    {
        scene->render(elapsed_time);
    }
}


void Scene_Manager::change_scene(Scene* next_scene)
{
    clear();

    scene = next_scene;
    scene->set_parent(this);

    if (!scene->is_ready())
    {
        scene->initialize();
    }
}


void Scene_Manager::clear()
{
    if (scene != nullptr)
    {
        scene->uninitialize();
        delete scene;
        scene = nullptr;
    }
}