#pragma once


#include "entity_manager.h"
#include "enemy.h"

namespace enemy_spawner_template
{
    template <typename T>
    constexpr bool false_v = false;
};

class Enemy_Spawner final
{
public:
    Enemy_Spawner(class Scene_Manager* ptr_scene_manager_) : ptr_scene_manager(ptr_scene_manager_){};
    ~Enemy_Spawner() = default;

    void set_enemies(class Scene_Manager* ptr_scene_manager);


    template<typename T>
    Enemy_Spawner& set_enemy(const DirectX::XMFLOAT3& position_, const DirectX::XMFLOAT3& target)
    {

        std::unique_ptr<Entity> enemy;

        if constexpr (std::is_base_of<Entity, T>{})
        {
            if constexpr (std::is_base_of<Enemy, T>{})
            {
                enemy = std::make_unique<T>(ptr_scene_manager, target);
            }
        }

        else
        {
            // 和訳 : この型はサポートされてないよ！！　せめてEntityを継承している型を使用してね！
            static_assert(enemy_spawner_template::false_v<T>, "This type is not supported.  Must be a type that inherits from Entity.");
        }

        
        assert(enemy.get());

        enemy->set_position(position_);

        Entity_Manager::instance().spawn_register(enemy);

        return *this;
    }

    template<typename T>
    Enemy_Spawner& set_enemy(const DirectX::XMFLOAT3& target)
    {

        std::unique_ptr<Entity> enemy;

        if constexpr (std::is_base_of<Entity, T>{})
        {
            if constexpr (std::is_base_of<Enemy, T>{})
            {
                enemy = std::make_unique<T>(ptr_scene_manager, target);
            }
        }

        else
        {
            // 和訳 : この型はサポートされてないよ！！　せめてEntityを継承している型を使用してね！
            static_assert(enemy_spawner_template::false_v<T>, "This type is not supported.  Must be a type that inherits from Entity.");
        }


        assert(enemy.get());
        
        Entity_Manager::instance().spawn_register(enemy);

        return *this;
    }

    void all_clear();   // 全敵を削除

private:
    class Scene_Manager* ptr_scene_manager = nullptr;
};

