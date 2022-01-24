
#include "enemy_spawner.h"

#include "ability.h"
#include "boss.h"

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
    
}

void Enemy_Spawner::set_enemy_2(const DirectX::XMFLOAT3& target)
{
    
}

void Enemy_Spawner::set_enemy_3(const DirectX::XMFLOAT3& target)
{
    
}

void Enemy_Spawner::set_enemy_4(const DirectX::XMFLOAT3& target)
{
    
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
