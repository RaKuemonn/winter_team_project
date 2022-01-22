#include "decotator_supporter.h"
#include "model_filepaths.h"
#include "scene_manager.h"
#include "imgui.h"

Decotator_Supporter::Decotator_Supporter(Scene_Manager* ptr_scene_manager_)
{
    m_model = std::make_unique<Model>(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Entity::ball));

    m_transform = std::unique_ptr<Transform>();
}

#ifdef _DEBUG
void Decotator_Supporter::imgui_control()
{
    ImGui::Begin("Decorator_Supporter");

    DirectX::XMFLOAT3 position  = m_transform->get_position();
    DirectX::XMFLOAT3 scale     = m_transform->get_scale();


    // imgui操作パラメーター群
    ImGui::InputFloat3("position", &position.x, "%.2f");
    ImGui::InputFloat3("scale   ", &scale.x, "%.2f");


    m_transform->set_position(position);
    m_transform->set_scale(scale);


    // 更新
    m_transform->Update();
    ImGui::End();
}

#else

void Decotator_Supporter::imgui_control()
{
    // Release時
    // なにもしない

}

#endif

