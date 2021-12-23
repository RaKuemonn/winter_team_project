
#include "collision_manager.h"
#include "collision.h"

#include "entity_manager.h"
#include "stage_manager.h"
#include "stage.h"


// レイの長さは 速度ではなく 決め打ち
inline bool ray_cast(std::weak_ptr<Entity> entity, Stage_Manager& s_manager, Hit_Result& hit_result_)
{
    const DirectX::XMFLOAT3 position    = entity.lock()->get_position();

    constexpr float offset_y = 50.0f;
    const DirectX::XMFLOAT3 start       = { position.x, position.y + offset_y, position.z };
    const DirectX::XMFLOAT3 end         = { position.x, position.y - offset_y, position.z };


    return s_manager.ray_cast(start, end, &hit_result_);
}

// 床へのレイキャスト
inline void ray_to_floor(
    Entity_Manager& e_manager,
    Stage_Manager& s_manager,
    std::vector<short>& vec_player_indices,
    std::vector<short>& vec_enemy_indices,
    std::vector<short>& vec_vehicle_indices)
{
    // プレイヤー
    Hit_Result result;
    for (auto index : vec_player_indices)
    {
        std::shared_ptr<Entity> player = e_manager.get_entity(index);
        if (ray_cast(player, s_manager, result))
        {
            player->set_position(result.position);
        }
    }

    // 敵
    result = {};
    for (auto index : vec_enemy_indices)
    {
        std::shared_ptr<Entity> enemy = e_manager.get_entity(index);
        if (ray_cast(enemy, s_manager, result))
        {
            enemy->set_position(result.position);
        }
    }

    // 乗り物
    result = {};
    for (auto index : vec_vehicle_indices)
    {
        std::shared_ptr<Entity> vehicle = e_manager.get_entity(index);
        if (ray_cast(vehicle, s_manager, result))
        {
            vehicle->set_position(result.position);
        }
    }
}



void Collision_Manager::judge()
{
    Entity_Manager& e_manager = Entity_Manager::instance();
    Stage_Manager& s_manager = Stage_Manager::instance();

    std::vector<short> vec_player_indices   = e_manager.get_entities(Tag::Player);
    std::vector<short> vec_enemy_indices    = e_manager.get_entities(Tag::Enemy);
    std::vector<short> vec_vehicle_indices  = e_manager.get_entities(Tag::Vehicle);

    ray_to_floor(e_manager, s_manager,
        vec_player_indices, vec_enemy_indices, vec_vehicle_indices);

}
