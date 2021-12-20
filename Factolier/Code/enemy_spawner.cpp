
#include "enemy_spawner.h"

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


void Enemy_Spawner::all_clear()
{
    Entity_Manager& manager = Entity_Manager::instance();

    const std::vector<short> vec_indices = manager.get_entity(Tag::Enemy);

    // entity_manager‚É‘¶İ‚·‚éEnemy‘S•”‚Ìíœ—\–ñ‚ğ‚·‚é
    Entity* enemy = nullptr;
    for (const auto& index : vec_indices)
    {
        enemy = static_cast<Enemy*>(manager.get_entity(index));

        // íœ—\–ñ
        manager.remove_register(enemy);
    }
}
