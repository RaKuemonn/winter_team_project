#include "decotator_supporter.h"
#include "model_filepaths.h"
#include "scene_manager.h"
#include "imgui.h"



#include "deco_egg.h"
#include "deco_leaf.h"
#include "deco_furniture.h"
#include "stage_manager.h"


Decotator_Supporter::Decotator_Supporter(Scene_Manager* ptr_scene_manager_)
{
    ptr_scene_manager = ptr_scene_manager_;

    m_model = std::make_unique<Model>(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Entity::ball, true));

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




    position = m_transform->get_position();

    if (ImGui::Button(Model_Paths::Deco::ball))
    {
        DirectX::XMFLOAT3 pboll1 = { -20,0, -10 };
        std::unique_ptr<Stage> s = std::make_unique<Deco_Ball>(ptr_scene_manager, pboll1);
        s->set_scale(10);
        Stage_Manager::instance().spawn_register(s);
    }
    if (ImGui::Button(Model_Paths::Deco::egg))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Egg>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::egg_2))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Egg2>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::aki_leaf))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Leaf_Fall>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::fuyu_leaf))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Leaf_Winter>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::kinoko))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Kinoko>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::pumpkin))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Pumpkin>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::tetrapod))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Tetrapos>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::snow_man))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Snow_Man>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::umbrella_and_bed))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Umbrella_and_Bed>(ptr_scene_manager, position));
    }
    if (ImGui::Button(Model_Paths::Deco::kamakura))
    {
        Stage_Manager::instance().spawn_register(std::make_unique<Deco_Kamakura>(ptr_scene_manager, position));
    }

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



