
#include "scene_game.h"
#include "scene_manager.h"
#include "camera.h"
#include "entity_manager.h"
#include "stage_1.h"
#include "stage_manager.h"
#include "player.h"
#include "enemy_none.h"
#include "camera_controller.h"


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
    player->set_position({ 0.0f, 5.0f, 0.0f });
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

    collision_manager->judge();
}


void Scene_Game::render(float elapsed_time) 
{
    //ID3D11DeviceContext* device_context_ = parent->device_context();

    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);

    Stage_Manager::instance().render(parent);
    Entity_Manager::instance().render(parent);
}
