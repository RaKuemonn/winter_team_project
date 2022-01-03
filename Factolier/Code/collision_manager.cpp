
#include "collision_manager.h"
#include "collision.h"

#include "entity_manager.h"
#include "stage_manager.h"
#include "stage.h"


// レイの長さは 速度ではなく 決め打ち
inline bool ray_cast(std::weak_ptr<Entity> entity, Stage_Manager& s_manager, Hit_Result& hit_result_)
{
    const DirectX::XMFLOAT3 velocity    = entity.lock()->get_velocity();

    // そもそも移動していなければ　早期returnさせる
    if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&velocity))) <= FLT_EPSILON) return false;

    // entityのこのフレームでの移動分からレイを作っている
    // すでに位置が更新されたあとなのでこの計算になっている entityの更新で速度更新->位置更新->床の当たり判定
    // 始点が前回位置、終点が現在位置

    const DirectX::XMFLOAT3 position    = entity.lock()->get_position();
    const DirectX::XMFLOAT3 start       = position - velocity;
    const DirectX::XMFLOAT3 end         = position/*{ position.x - velocity.x, position.y, position.z - velocity.z }*/;


    return s_manager.ray_cast(start, end, &hit_result_);
}

// 球体の乗り物用にオーバーロードしたraycast関数
inline bool ray_cast(std::weak_ptr<Entity> entity, const DirectX::XMFLOAT3& scale, Stage_Manager& s_manager, Hit_Result& hit_result_)
{
    const DirectX::XMFLOAT3 velocity = entity.lock()->get_velocity();

    // そもそも移動していなければ　早期returnさせる
    if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&velocity))) <= FLT_EPSILON) return false;

    // entityのこのフレームでの移動分からレイを作っている
    // すでに位置が更新されたあとなのでこの計算になっている entityの更新で速度更新->位置更新->床の当たり判定
    // 始点が前回位置、終点が現在位置

    constexpr float offset_y = 5.0f;
    const DirectX::XMFLOAT3 position                = entity.lock()->get_position();
    const DirectX::XMFLOAT3 tread                   = { position.x,position.y - scale.y, position.z }; // tread : 足下
    const DirectX::XMFLOAT3 start                   = { tread.x - velocity.x, tread.y - velocity.y + offset_y, tread.z - velocity.z };
    const DirectX::XMFLOAT3 end                     = tread/*{ position.x - velocity.x, position.y, position.z - velocity.z }*/;


    return s_manager.ray_cast(start, end, &hit_result_);
}



// 別名エイリアス
using vec_type_short = std::vector<short>;
using vectors = std::tuple<vec_type_short, vec_type_short, vec_type_short>;







// 床へのレイキャスト    [ entity_managerに登録している "" プレイヤー、敵、乗り物 "" が処理されている ]
inline void ray_to_floor(
    Entity_Manager& e_manager,
    Stage_Manager& s_manager,
    vectors& vectors_)
{
    // プレイヤー
    Hit_Result result;
    for (auto index : std::get<0>(vectors_))
    {
        std::shared_ptr<Entity> player = e_manager.get_entity(index);
        if (ray_cast(player, s_manager, result))
        {
            player->set_position(result.position);
            player->set_velocity_y(0.0f);
        }
    }

    // 敵
    result = {};
    for (auto index : std::get<1>(vectors_))
    {
        std::shared_ptr<Entity> enemy = e_manager.get_entity(index);
        if (ray_cast(enemy, s_manager, result))
        {
            enemy->set_position(result.position);
            enemy->set_velocity_y(0.0f);
        }
    }

    // 乗り物
    result = {};
    for (auto index : std::get<2>(vectors_))
    {
        std::shared_ptr<Entity>     vehicle = e_manager.get_entity(index);
        const DirectX::XMFLOAT3&    scale   = vehicle->get_scale();

        if (ray_cast(vehicle, scale, s_manager, result))
        {
            // 乗り物(球体)の半径(スケール)分上へ加算する処理をする
            //
            // 理由 : 球のモデル原点が球の中心にあるので、
            //        resultの値をそのまま使うと球がめり込んだ状態でset_positionされてしまうから


#if 1   // ここを0と１で切り替えると結果処理が変わるよ
            result.position.y += scale.y;
#endif


            vehicle->set_position(result.position);
            vehicle->set_velocity_y(0.0f);
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

    vectors vectors_ = {
        vec_player_indices,
        vec_enemy_indices,
        vec_vehicle_indices
    };

    // 床へのレイキャスト
    ray_to_floor(
        e_manager, s_manager,
        vectors_
    );

}
