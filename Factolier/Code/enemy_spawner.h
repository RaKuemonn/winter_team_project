#pragma once


#include "entity_manager.h"
#include "enemy.h"

template <typename T>
constexpr bool false_v = false;


class Enemy_Spawner final
{
public:
    Enemy_Spawner(class Scene_Manager* ptr_scene_manager_) : ptr_scene_manager(ptr_scene_manager_){};
    ~Enemy_Spawner() = default;

    void set_enemies(class Scene_Manager* ptr_scene_manager);


    template<typename T>
    void set_enemy(const DirectX::XMFLOAT3& position_)
    {

        std::unique_ptr<Entity> enemy;

        if constexpr (std::is_base_of<Entity, T>{})
        {
            if constexpr (std::is_base_of<Enemy, T>{})
            {
                enemy = std::make_unique<T>(ptr_scene_manager);
            }
        }

        else
        {
            // �a�� : ���̌^�̓T�|�[�g����ĂȂ���I�I�@���߂�Entity���p�����Ă���^���g�p���ĂˁI
            static_assert(false_v<T>, "This type is not supported.  Must be a type that inherits from Entity.");
        }

        
        if (enemy.get() == nullptr)return;

        enemy->set_position(position_);

        Entity_Manager::instance().spawn_register(enemy);
    }

    void all_clear();   // �S�G���폜

private:
    class Scene_Manager* ptr_scene_manager = nullptr;
};

