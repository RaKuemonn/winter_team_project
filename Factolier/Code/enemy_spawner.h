#pragma once


#include "entity_manager.h"

class Enemy_Spawner final
{
public:
    Enemy_Spawner() = default;
    ~Enemy_Spawner() = default;

    void set_enemies(class Scene_Manager* ptr_scene_manager);

    template<typename T>
    void set_enemy(const DirectX::XMFLOAT3& position_)
    {

        constexpr if(T )
        std::unique_ptr<T> 

        //Entity_Manager::instance().spawn_register()
    }

    void all_clear();   // ‘S“G‚ğíœ
};

