
#include "enemy_spawner.h"

#include "ability.h"
#include "boss.h"
#include "enemy_spring.h"
#include "enemy_summer.h"
#include "enemy_winter.h"
#include "enemy_fall.h"

void Enemy_Spawner::set_enemies(Scene_Manager* ptr_scene_manager)
{
    Entity_Manager& manager = Entity_Manager::instance();

    //manager.spawn_register(std::make_unique<Enemy>(ptr_scene_manager));
    //manager.spawn_register(std::make_unique<Enemy>(ptr_scene_manager));
    //manager.spawn_register(std::make_unique<Enemy>(ptr_scene_manager));
    //manager.spawn_register(std::make_unique<Enemy>(ptr_scene_manager));
    //manager.spawn_register(std::make_unique<Enemy>(ptr_scene_manager));
    //manager.spawn_register(std::make_unique<Enemy>(ptr_scene_manager));
}


void Enemy_Spawner::set_enemy_1(const DirectX::XMFLOAT3& target)
{
    set_enemy<Enemy_Spring>({ 0.0f,-7.6f,-72.0f }, target)->set_area_size({ 40.0f,0.5f,30.0f });
}

void Enemy_Spawner::set_enemy_2(const DirectX::XMFLOAT3& target)
{
    set_enemy<Enemy_Summer>({ 0.0f,13.6f,-10.0f }, target)->set_area_size({ 14.0f,0.5f,2.0f });
    set_enemy<Enemy_Summer>({ 0.0f,10.2f,-38.6f }, target)->set_area_size({ 14.0f,0.5f,8.0f });
}

void Enemy_Spawner::set_enemy_3(const DirectX::XMFLOAT3& target)
{
    set_enemy<Enemy_Fall>({ 0.0f,-8.6f,-92.0f }, target)->set_area_size({ 10.0f,0.5f,10.0f });
    set_enemy<Enemy_Fall>({ -4.0f,-8.2f,-43.0f }, target)->set_area_size({ 5.0f,0.5f,10.0f });
    set_enemy<Enemy_Fall>({ 4.0f,-8.2f,-43.0f }, target)->set_area_size({ 5.0f,0.5f,10.0f });
    set_enemy<Enemy_Fall>({ -9.2f,-8.2f,45.0f }, target)->set_area_size({ 10.0f,0.5f,5.0f });
    set_enemy<Enemy_Fall>({ 0.0f,-8.2f,-45.0f }, target)->set_area_size({ 10.0f,0.5f,5.0f });
    set_enemy<Enemy_Fall>({ 9.2f,-8.2f,-45.0f }, target)->set_area_size({ 10.0f,0.5f,5.0f });
}

void Enemy_Spawner::set_enemy_4(const DirectX::XMFLOAT3& target)
{
    set_enemy<Enemy_Winter>({ 0.0f,0.7f,-95.0f }, target)->set_area_size({ 10.0f,0.5f,10.0f });
    set_enemy<Enemy_Winter>({ 0.0f,8.0f,-46.0f }, target)->set_area_size({ 10.0f,0.5f,8.0f });
    set_enemy<Enemy_Winter>({ 0.0f,6.4f,67.0f },  target)->set_area_size({ 16.0f,0.5f,10.0f });
}

short* Enemy_Spawner::set_enemy_boss(const DirectX::XMFLOAT3& target)
{
    short* ptr_boss_hp = nullptr;

    ptr_boss_hp = set_enemy<Boss>(target)->get_ability().get_ptr_hp();

    return ptr_boss_hp;
}




void Enemy_Spawner::all_clear()
{
    Entity_Manager& manager = Entity_Manager::instance();

    const std::vector<short> vec_indices = manager.get_entities(Tag::Enemy);

    // entity_manager‚É‘¶İ‚·‚éEnemy‘S•”‚Ìíœ—\–ñ‚ğ‚·‚é
    Entity* enemy = nullptr;
    for (const auto& index : vec_indices)
    {
        enemy = static_cast<Enemy*>(manager.get_entity(index).get());

        // íœ—\–ñ
        manager.remove_register(enemy);
    }
}
