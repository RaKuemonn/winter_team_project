#include "decotator_supporter.h"
#include "model_filepaths.h"
#include "scene_manager.h"
#include "imgui.h"

Decotator_Supporter::Decotator_Supporter(Scene_Manager* ptr_scene_manager_)
{
    m_model = std::make_unique<Model>(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Entity::ball));

    m_transform = std::make_unique<Transform>();
    constexpr float scale = 0.5f;
    m_transform->set_scale({ scale,scale,scale });
    m_transform->Update();

    m_range = { -20.0f,+20.0f };
}

#ifdef _DEBUG
void Decotator_Supporter::imgui_control()
{
    ImGui::Begin("Decorator_Supporter");

    DirectX::XMFLOAT3 position  = m_transform->get_position();
    DirectX::XMFLOAT3 scale     = m_transform->get_scale();


    // imgui操作パラメーター群
    //ImGui::InputFloat3("position", &position.x, "%.2f");
    //m_transform->set_position(position);
    //position = m_transform->get_position();

    ImGui::InputFloat2("slider_range", &m_range.x, "%.3f");

    ImGui::SliderFloat3("position", &position.x, m_range.x, m_range.y, "%.2f");
    m_transform->set_position(position);


    //ImGui::InputFloat3("scale   ", &scale.x, "%.3f");
    //m_transform->set_scale(scale);
    //scale = m_transform->get_scale();
    ImGui::SliderFloat3("scale   ", &scale.x, 0.01f, 2.0f, "%.3f");
    m_transform->set_scale(scale);
    


    // 更新
    m_transform->Update();
    ImGui::End();
}


void Decotator_Supporter::render(ID3D11DeviceContext* device_context)
{
    m_model->render(device_context, m_transform->get_matrix(), { 1.0f, 1.0f, 1.0f, 1.0f });
}

#else

void Decotator_Supporter::imgui_control()
{
    // Release時
    // なにもしない

}

void Decotator_Supporter::render(ID3D11DeviceContext* device_context)
{

}

#endif



