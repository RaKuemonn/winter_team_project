
#include "scene_game.h"
#include "scene_manager.h"
#include "shader_manager.h"
#include "camera.h"
#include "entity_manager.h"
#include "stage_1.h"
#include "stage_1_movement.h"
#include "stage_manager.h"
#include "player.h"
#include "enemy_none.h"
#include "camera_controller.h"
#include "imgui.h"

inline void imgui()
{
#ifdef _DEBUG
    ImGui::Begin("Vehicle");

    std::vector<short> vec = Entity_Manager::instance().get_entities(Tag::Vehicle);
    const int size = static_cast<int>(vec.size());

    for(int i = 0; i < size; ++i)
    {
        ImGui::Text("Number %d", i);

        Entity* entity = Entity_Manager::instance().get_entity(vec.at(i)).get();

        if (entity)
        {
            DirectX::XMFLOAT3 velo = entity->get_velocity();
            ImGui::InputFloat3("velocity", &velo.x);
            float velo_length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMLoadFloat3(&velo)));
            ImGui::InputFloat("velocity_length", &velo_length);

            if(velo_length > 3.0f)
            {
                int a = 1;
                a++;
            }

        }
    }
    
    ImGui::End();
#endif
}


void Scene_Game::initialize()
{

    Camera& camera = Camera::Instance();
    camera.set_lookat(
        DirectX::XMFLOAT3(0, 10, -10),
        DirectX::XMFLOAT3(0, 0, 0),
        DirectX::XMFLOAT3(0, 1, 0)
    );
    camera.set_perspective_fov(DirectX::XMConvertToRadians(90),
        1280 / 720,
        0.1f,
        1000.0f);




    std::unique_ptr<Entity> player = std::make_unique<Player>(parent);
    //player->set_position({ 0.0f, 5.0f, 0.0f });
    player->set_position({ 0.0f, -92.0f, -640.0f });
    player->init();
    camera_controller = std::make_unique<Camera_Controller>(&player->get_position());

    Entity_Manager::instance().spawn_register(player);
    enemy_spawner = std::make_unique<Enemy_Spawner>(parent);
    enemy_spawner->set_enemy<Enemy_None>({ 4.0f,5.0f,1.0f });
    enemy_spawner->set_enemy<Enemy_None>({ 8.0f,5.0f,1.0f });
    enemy_spawner->set_enemy<Enemy_None>({ -4.0f,5.0f,1.0f });
    enemy_spawner->set_enemy<Enemy_None>({ -8.0f,5.0f,1.0f });

    collision_manager = std::unique_ptr<Collision_Manager>();

    Stage_Manager::instance().spawn_register(std::make_unique<Stage_1>(parent));
    Stage_Manager::instance().spawn_register(std::make_unique<Stage_1_Movement>(parent));
}


void Scene_Game::uninitialize()
{
    Entity_Manager::instance().all_clear();
    Stage_Manager::instance().all_clear();
}


void Scene_Game::update(float elapsed_time)
{
    Stage_Manager::instance().update(elapsed_time);

    Entity_Manager::instance().update(elapsed_time);


    camera_controller->update(elapsed_time,nullptr);

    collision_manager->judge(elapsed_time);

    imgui();
}


void Scene_Game::render(float elapsed_time) 
{

    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);


    Shader* shader = nullptr;
    ID3D11DeviceContext* ptr_device_context = parent->device_context();
    //シャドウマップ生成
    {
        shader = parent->shader_manager()->get_shader(Shaders::SHADOW);

        shader->begin(ptr_device_context, elapsed_time * 0.1f);

        Entity_Manager::instance().render(ptr_device_context);

        shader->end(ptr_device_context);
    }

    //レンダーターゲットビューと深度ステンシルビューを元に戻す
    {
        ID3D11RenderTargetView* rtv = parent->render_target_view();
        ID3D11DepthStencilView* dsv = parent->depth_stencil_view();
        FLOAT color[]{ 1.0f, 1.0f, 1.0f, 1.0f };

        ptr_device_context->ClearRenderTargetView(rtv, color);
        ptr_device_context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        ptr_device_context->OMSetRenderTargets(1, &rtv, dsv);
    }


    parent->state_manager()->setDS(DS::ON_ON);
    shader = parent->shader_manager()->get_shader(Shaders::PHONG);

    shader->begin(ptr_device_context);

    Stage_Manager::instance().render(ptr_device_context);
    Entity_Manager::instance().render(ptr_device_context);

    shader->end(ptr_device_context);
}
