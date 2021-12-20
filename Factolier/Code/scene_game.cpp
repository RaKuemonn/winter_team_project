
#include "scene_game.h"
#include "scene_manager.h"
#include "camera.h"
#include "entity_manager.h"
#include "sv_ball.h"

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
    
    Entity_Manager::instance().spawn_register(std::make_unique<Player>(parent));
    //Entity_Manager::instance().spawn_register(std::make_unique<SV_Ball>(parent));
    //Entity_Manager::instance().spawn_register(std::make_unique<SV_Ball>(parent));
    //Entity_Manager::instance().spawn_register(std::make_unique<SV_Ball>(parent));
    //Entity_Manager::instance().spawn_register(std::make_unique<Player>(parent));
    //Entity_Manager::instance().spawn_register(std::make_unique<SV_Ball>(parent));

    std::vector<short> vec_tag = {};

    vec_tag = Entity_Manager::instance().get_entity(Tag::Player);

    int data = vec_tag.size();
}


void Scene_Game::uninitialize()
{
    Entity_Manager::instance().all_clear();
}


void Scene_Game::update(float elapsedTime)
{
    Entity_Manager::instance().update(elapsedTime);
}


void Scene_Game::render(float elapsedTime)
{
    ID3D11DeviceContext* device_context_ = parent->device_context();

    parent->state_manager()->setSS(SS::POINT);
    parent->state_manager()->setSS(SS::LINEAR);
    parent->state_manager()->setSS(SS::ANISOTROPIC);

    parent->state_manager()->setDS(DS::OFF_OFF);

    parent->state_manager()->setBS(BS::ALPHA);

    parent->state_manager()->setRS(RS::SOLID_NONE);


    Entity_Manager::instance().render();
}
