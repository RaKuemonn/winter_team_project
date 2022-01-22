﻿
#include "collision_manager.h"
#include "collision.h"

#include "entity_manager.h"
#include "imgui.h"
#include "sphere_vehicle.h"
#include "stage_manager.h"
#include "stage.h"


// ローカル関数
namespace local_functions
{
    // 符号判定用のローカル関数 (正1.0f か 負-1.0f, ゼロ0.0f)が返る
    inline float sign(const float value_)
    {
        return static_cast<float>((value_ > 0.0f) - (value_ < 0.0f));
    };
    // Entityが範囲外なら削除予約する関数
    inline void judge_out_range(std::weak_ptr<Entity> entity, Entity_Manager& e_manager)
    {
        const float length = DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&entity.lock()->get_position()), DirectX::XMVectorZero())));

        constexpr float out_range = 500.0f;
        if (length < out_range) return;;

        e_manager.remove_register(entity.lock().get());
    }

};
// 名前省略
using namespace local_functions;


// レイを用いた衝突判定の関数群
namespace ray_functions
{
    // レイの長さは 速度
    inline bool ray_cast_floor(const float elapsed_time, std::weak_ptr<Entity> entity, Stage_Manager& s_manager, Hit_Result& hit_result_)
    {
        const DirectX::XMFLOAT3 velocity = entity.lock()->get_velocity() * elapsed_time;

        // そもそも移動していなければ　早期returnさせる
        //if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&velocity))) <= FLT_EPSILON) return false;

        // 速度が下向き以外は早期return
        if (velocity.y >= 0.0f) return false;

        // entityのこのフレームでの移動分からレイを作っている
        // すでに位置が更新されたあとなのでこの計算になっている entityの更新で速度更新->位置更新->床の当たり判定
        // 始点が前回位置、終点が現在位置

        const DirectX::XMFLOAT3 position = entity.lock()->get_position();
        const float scale_y = entity.lock()->get_scale().y;
        const DirectX::XMFLOAT3 start = { position.x, position.y - velocity.y + scale_y * 0.5f, position.z };
        const DirectX::XMFLOAT3 end = position/*{ position.x - velocity.x, position.y, position.z - velocity.z }*/;


        return s_manager.ray_cast(start, end, &hit_result_);
    }

    // 球体の乗り物用にオーバーロードしたraycast関数
    inline bool ray_cast_floor(const float elapsed_time, std::weak_ptr<Entity> entity, const DirectX::XMFLOAT3& scale, Stage_Manager& s_manager, Hit_Result& hit_result_)
    {
        const DirectX::XMFLOAT3 velocity = entity.lock()->get_velocity() * elapsed_time;

        // 移動床の関係でこの処理はコメントアウト
        // // そもそも移動していなければ　早期returnさせる
        // if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&velocity))) <= FLT_EPSILON) return false;

        // 速度が下向き以外は早期return
        if (velocity.y >= 0.0f) return false;

        // entityのこのフレームでの移動分からレイを作っている
        // すでに位置が更新されたあとなのでこの計算になっている entityの更新で速度更新->位置更新->床の当たり判定
        // 始点が前回位置、終点が現在位置

        const float offset_y = scale.y * 0.5f;
        const float scale_epsilon = static_cast<Sphere_Vehicle*>(entity.lock().get())->get_is_free() ? SPHERE_SCALE_DECREASE * elapsed_time : 0.0f;

        const DirectX::XMFLOAT3 position = entity.lock()->get_latest_position();    //TODO: latest_position
        const DirectX::XMFLOAT3 tread = { position.x,position.y - scale.y + scale_epsilon, position.z };                                        // tread : 足下   (現在位置)

        const DirectX::XMFLOAT3 start   = { tread.x/* - velocity.x*/, tread.y - velocity.y + offset_y, tread.z/* - velocity.z*/ };   //                (前回位置)
        const DirectX::XMFLOAT3 end     = tread;


        return s_manager.ray_cast(start, end, &hit_result_);
    }


    inline bool ray_cast_wall(const float elapsed_time, std::weak_ptr<Entity> entity, Stage_Manager& s_manager, Hit_Result& hit_result_, DirectX::XMFLOAT3& wall_vec)
    {
        const DirectX::XMFLOAT3 velocity    = entity.lock()->get_velocity() * elapsed_time;

        // そもそも移動していなければ　早期returnさせる
        //if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&velocity))) <= FLT_EPSILON) return false;

        const DirectX::XMFLOAT3 position    = entity.lock()->get_position();
        const DirectX::XMFLOAT3 scale       = entity.lock()->get_scale();

        const DirectX::XMFLOAT3 offset      = { -1.0f * sign(velocity.x) * scale.x * 0.13f,
                                                0.0f,
                                                -1.0f * sign(velocity.z) * scale.z * 0.13f };

        const DirectX::XMFLOAT3 start       = { position.x - velocity.x + offset.x,
                                                    position.y,
                                                position.z - velocity.z + offset.z };
        const DirectX::XMFLOAT3 end         = position;

        if(s_manager.ray_cast(start, end, &hit_result_))
        {
            const DirectX::XMFLOAT3 origin_start = start - offset;
            const DirectX::XMVECTOR xmvec = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&end), DirectX::XMLoadFloat3(&origin_start));

            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit_result_.normal);
            DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(xmvec, DirectX::XMVectorScale(Normal, DirectX::XMVectorGetX(DirectX::XMVector3Dot(xmvec, Normal)))));
            return true;
        }

        return false;
    }

    inline bool ray_cast_wall(const float elapsed_time, std::weak_ptr<Entity> entity, const DirectX::XMFLOAT3& scale, Stage_Manager& s_manager, Hit_Result& hit_result_, DirectX::XMFLOAT3& wall_vec)
    {

        const DirectX::XMFLOAT3 velocity = entity.lock()->get_velocity() * elapsed_time;

        // そもそも移動していなければ　早期returnさせる
        //if (DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&velocity))) <= FLT_EPSILON) return false;

        const DirectX::XMFLOAT3 position = entity.lock()->get_position(); //TODO: latest_position
        const DirectX::XMFLOAT3 detect = { position.x - velocity.x,        position.y, position.z - velocity.z };
        

        // 一回目 x
        DirectX::XMVECTOR xmvec_x;
        const float offset_x = scale.x * sign(velocity.x);
        const float pudding_x = -1.0f * offset_x * 0.5f;
        {
            const DirectX::XMFLOAT3 start   = { detect.x + offset_x + pudding_x,    detect.y,       detect.z };
            const DirectX::XMFLOAT3 end     = { position.x + offset_x,              position.y,     position.z };

            // レイキャスト
            s_manager.ray_cast(start, end, &hit_result_);

            
            const DirectX::XMFLOAT3 origin_start = { start.x - pudding_x, start.y, start.z };
            xmvec_x = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&end), DirectX::XMLoadFloat3(&origin_start));
        }

        Hit_Result result = {}; // 比較用
        // 二回目 z
        const float offset_z = scale.z * sign(velocity.z);
        const float pudding_z   = -1.0f * offset_z * 0.5f;
        DirectX::XMVECTOR xmvec_z;
        {
            const DirectX::XMFLOAT3 start   = { detect.x,   detect.y,   detect.z + offset_z + pudding_z };
            const DirectX::XMFLOAT3 end     = { position.x, position.y, position.z + offset_z };

            // レイキャスト
            s_manager.ray_cast(start, end, &result);

            const DirectX::XMFLOAT3 origin_start = { start.x, start.y, start.z - pudding_z };
            xmvec_z = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&end), DirectX::XMLoadFloat3(&origin_start));
        }

        // 当たり判定の結果判断
        if (result.material_index + hit_result_.material_index > -2)
        {

#ifdef _DEBUG
            std::wstring axis;
#endif

            /*if(result.material_index >= 0 && hit_result_.material_index >= 0)
            {
                axis = L"x & z\n";


                DirectX::XMVECTOR x_Normal = DirectX::XMLoadFloat3(&hit_result_.normal);
                DirectX::XMVECTOR z_Normal = DirectX::XMLoadFloat3(&result.normal);
                DirectX::XMVECTOR x_vec = DirectX::XMVectorSubtract(xmvec_1, DirectX::XMVectorScale(x_Normal, DirectX::XMVectorGetX(DirectX::XMVector3Dot(xmvec_1, x_Normal))));
                DirectX::XMVECTOR z_vec = DirectX::XMVectorSubtract(xmvec_2, DirectX::XMVectorScale(z_Normal, DirectX::XMVectorGetX(DirectX::XMVector3Dot(xmvec_2, z_Normal))));

                DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorAdd(x_vec,z_vec));

            }

            else */
            if (result.distance < hit_result_.distance)
            {
#ifdef _DEBUG 
                axis = L"z\n";
#endif
                hit_result_ = result;
                DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit_result_.normal);
                DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(xmvec_z, DirectX::XMVectorScale(Normal,DirectX::XMVectorGetX(DirectX::XMVector3Dot(xmvec_z, Normal)))));

            }
            else
            {
#ifdef _DEBUG
                axis = L"x\n";
#endif
                DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit_result_.normal);
                DirectX::XMStoreFloat3(&wall_vec, DirectX::XMVectorSubtract(xmvec_x, DirectX::XMVectorScale(Normal,DirectX::XMVectorGetX(DirectX::XMVector3Dot(xmvec_x, Normal)))));

            }

            // レイの結果を出力ウィンドウに出力
#if _DEBUG
            if (entity.lock().get()->get_tag() == Tag::Vehicle)
            {
                if (static_cast<Sphere_Vehicle*>(entity.lock().get())->get_is_free() == false)
                {
                    using namespace std;


                    //const DirectX::XMFLOAT3 start = { detect.x + offset_x + pudding_x,    detect.y,       detect.z };
                    //const DirectX::XMFLOAT3 end = { position.x + offset_x,              position.y,     position.z };
                    const DirectX::XMFLOAT3 start = { detect.x,   detect.y,   detect.z + offset_z + pudding_z };
                    const DirectX::XMFLOAT3 end = { position.x, position.y, position.z + offset_z };

                    //
                    std::wstring out1 = std::to_wstring(start.x) + L" , ";
                    std::wstring out2 = std::to_wstring(start.y) + L" , ";
                    std::wstring out3 = std::to_wstring(start.z) + L"\n";
                    //
                    std::wstring out4 = std::to_wstring(end.x) + L" , ";
                    std::wstring out5 = std::to_wstring(end.y) + L" , ";
                    std::wstring out6 = std::to_wstring(end.z) + L"\n";

                    std::wstring out7 = std::to_wstring(position.x - velocity.x + wall_vec.x) + L" , ";
                    std::wstring out8 = std::to_wstring(position.y) + L" , ";
                    std::wstring out9 = std::to_wstring(position.z - velocity.z + wall_vec.z) + L"\n";;

                    std::wstring out10 = std::to_wstring(hit_result_.position.x) + L" , ";
                    std::wstring out11 = std::to_wstring(hit_result_.position.y) + L" , ";
                    std::wstring out12 = std::to_wstring(hit_result_.position.z) + L"\n";

                    std::wstring outputMe = axis + L"start " + out1 + out2 + out3 + L"end   " + out4 + out5 + out6 + L"result" + out7 + out8 + out9 + L"hit   " + out10 + out11 + out12 + L"\n\n";
                    OutputDebugString(outputMe.c_str());
                }
            }
#endif

            return true;
        }

        return false;
    }


}


// 立体図形を用いた衝突判定の関数群
namespace detect_functions
{
    inline bool sphere_vs_sphere(const DirectX::XMFLOAT3& pos_a, const DirectX::XMFLOAT3& pos_b, const float radius_a, const float radius_b)
    {
        const float length_sq = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&pos_b), DirectX::XMLoadFloat3(&pos_a))));
        const float sum_radius = radius_a + radius_b;

        return (length_sq < (sum_radius* sum_radius));
    }

    inline bool sphere_vs_sphere(std::weak_ptr<Entity> entity_a_, std::weak_ptr<Entity> entity_b_)
    {
        const float radius_a = (entity_a_.lock()->get_tag() == Tag::Vehicle) ? entity_a_.lock()->get_scale().x : 0.5f;
        const float radius_b = (entity_b_.lock()->get_tag() == Tag::Vehicle) ? entity_b_.lock()->get_scale().x : 0.5f;

        return sphere_vs_sphere(entity_a_.lock()->get_position(), entity_b_.lock()->get_position(), radius_a, radius_b);
    }

    inline void sphere_vs_sphere_extrusion(std::weak_ptr<Entity> entity_a_, std::weak_ptr<Entity> entity_b_, const float elapsed_time_)
    {
        const DirectX::XMVECTOR velocity_a = DirectX::XMLoadFloat3(&entity_a_.lock()->get_velocity());
        const DirectX::XMVECTOR velocity_b = DirectX::XMLoadFloat3(&entity_b_.lock()->get_velocity());


        const DirectX::XMFLOAT3& xmf3_position_a = entity_a_.lock()->get_position();
        const DirectX::XMFLOAT3& xmf3_position_b = entity_b_.lock()->get_position();
        //const DirectX::XMVECTOR position_a = DirectX::XMLoadFloat3(&xmf3_position_a);
        //const DirectX::XMVECTOR position_b = DirectX::XMLoadFloat3(&xmf3_position_b);
        const DirectX::XMVECTOR position_a = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&xmf3_position_a),DirectX::XMVectorScale(velocity_a,elapsed_time_));
        const DirectX::XMVECTOR position_b = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&xmf3_position_b),DirectX::XMVectorScale(velocity_b,elapsed_time_));


        // a to b と b to a の単位ベクトル
        const DirectX::XMVECTOR a_to_b_dir = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(position_b, position_a));
        const DirectX::XMVECTOR b_to_a_dir = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(position_a, position_b));
#if 1


        // ぶつかったときの衝撃の大きさを計算
        const float impact_length_a = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(velocity_a, elapsed_time_), a_to_b_dir));
        const float impact_length_b = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorScale(velocity_b, elapsed_time_), b_to_a_dir));

        // 衝撃のベクトルを計算
        const DirectX::XMVECTOR a_impact_vec = DirectX::XMVectorScale(b_to_a_dir, impact_length_b);
        const DirectX::XMVECTOR b_impact_vec = DirectX::XMVectorScale(a_to_b_dir, impact_length_a);
        

        DirectX::XMFLOAT3 xmf3_calc_velocity_a;
        DirectX::XMFLOAT3 xmf3_calc_velocity_b;
        DirectX::XMStoreFloat3(&xmf3_calc_velocity_a, DirectX::XMVectorAdd(velocity_a, a_impact_vec));
        DirectX::XMStoreFloat3(&xmf3_calc_velocity_b, DirectX::XMVectorAdd(velocity_b, b_impact_vec));

        // 速度の修正
        entity_a_.lock()->set_velocity(xmf3_calc_velocity_a);
        entity_b_.lock()->set_velocity(xmf3_calc_velocity_b);
#endif



        // 位置の修正
        const float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(position_b, position_a)));
        
        float radius_a = 0.5f;
        if (entity_a_.lock()->get_tag() == Tag::Vehicle)
        {
            const float difference_a = ((static_cast<Sphere_Vehicle*>(entity_a_.lock().get())->get_is_free()) ? SPHERE_SCALE_DECREASE * elapsed_time_ : 0.0f);
            radius_a = entity_a_.lock()->get_scale().x + difference_a;
        }
        
        float radius_b = 0.5f;
        if (entity_b_.lock()->get_tag() == Tag::Vehicle)
        {
            const float difference_b = ((static_cast<Sphere_Vehicle*>(entity_b_.lock().get())->get_is_free()) ? SPHERE_SCALE_DECREASE * elapsed_time_ : 0.0f);
            radius_b = entity_b_.lock()->get_scale().x + difference_b;
        }

        //const float radius_a = (entity_a_.lock()->get_tag() == Tag::Vehicle) ? entity_a_.lock()->get_scale().x * 0.5f : 0.5f;
        //const float radius_b = (entity_b_.lock()->get_tag() == Tag::Vehicle) ? entity_b_.lock()->get_scale().x * 0.5f : 0.5f;

        const float sum_radius = radius_a + radius_b;
        
        const float mass_a                  = entity_a_.lock()->get_mass();
        const float mass_b                  = entity_b_.lock()->get_mass();
        //const float penetration             = sum_radius - length;
        const float penetration             = sum_radius - length;
        const float half_sum_penetration    = penetration / (mass_a + mass_b);
        const float penetration_a           = mass_a * half_sum_penetration;
        const float penetration_b           = mass_b * half_sum_penetration;
        //const float penetration_a           = penetration * 0.5f;
        //const float penetration_b           = penetration * 0.5f;

        DirectX::XMFLOAT3 new_position_a;
        DirectX::XMFLOAT3 new_position_b;
        DirectX::XMStoreFloat3(&new_position_a, DirectX::XMVectorAdd(position_a, DirectX::XMVectorScale(a_to_b_dir, penetration_a)));
        DirectX::XMStoreFloat3(&new_position_b, DirectX::XMVectorAdd(position_b, DirectX::XMVectorScale(b_to_a_dir, penetration_b)));

        entity_a_.lock()->set_position(new_position_a);
        entity_b_.lock()->set_position(new_position_b);
    }

    inline bool box_vs_box(std::weak_ptr<Entity> entity, std::weak_ptr<Entity> entity_out)
    {
        // TODO: 未定義
    }

}



// 別名エイリアス
using vec_type_short    = std::vector<short>;
using vectors           = std::tuple<vec_type_short, vec_type_short, vec_type_short>;

// ローカルの名前空間名省略
using namespace ray_functions;
using namespace detect_functions;



// 床へのレイキャスト    [ entity_managerに登録している "" プレイヤー、敵、乗り物 "" が処理されている ]
inline void ray_to_floor(
    const float elapsed_time,
    Entity_Manager& e_manager,
    Stage_Manager& s_manager,
    const vectors& vectors_)
{
    // 交差結果用のローカル変数
    Hit_Result result;

    // 摩擦係数の定数
    // TODO : 現在固定値だが、床によって摩擦係数が変わるようにする
    constexpr float friction_ratio = 0.1f;



    // プレイヤー
    for (auto index : std::get<0>(vectors_))
    {
        std::shared_ptr<Entity> player = e_manager.get_entity(index);
        player->set_friction(0.0f);

        if (ray_cast_floor(elapsed_time,player, s_manager, result))
        {
            player->set_position(result.position);
            player->set_velocity_y(0.0f);
            player->set_friction(friction_ratio);
        }
    }
    
    // 敵
    for (auto index : std::get<1>(vectors_))
    {
        std::shared_ptr<Entity> enemy = e_manager.get_entity(index);
        enemy->set_friction(0.0f);
        result = {};
        if (ray_cast_floor(elapsed_time, enemy, s_manager, result))
        {
            enemy->set_position(result.position);
            enemy->set_velocity_y(0.0f);
            enemy->set_friction(friction_ratio);
        }
    }

    // 乗り物
    for (auto index : std::get<2>(vectors_))
    {
        std::shared_ptr<Entity>     vehicle = e_manager.get_entity(index);
        const DirectX::XMFLOAT3&    scale   = vehicle->get_scale();
        vehicle->set_friction(0.0f);
        result = {};
        if (ray_cast_floor(elapsed_time, vehicle, scale, s_manager, result))
        {
            // 乗り物(球体)の半径(スケール)分上へ加算する処理をする
            //
            // 理由 : 球のモデル原点が球の中心にあるので、
            //        resultの値をそのまま使うと球がめり込んだ状態でset_positionされてしまうから



#if 1   // ここを0と１で切り替えると結果処理が変わるよ
            result.position.y += scale.y;
#endif
            if (result.rotation.y > 0.0f)
            {
                DirectX::XMFLOAT4 quaternion;
                DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYaw(0.0f, result.rotation.y, 0.0f)));
                vehicle->add_quaternion(quaternion);
            }

            vehicle->set_position(result.position);
            vehicle->set_velocity_y(0.0f);
            vehicle->set_friction(friction_ratio);

            if (static_cast<Sphere_Vehicle*>(vehicle.get())->get_on_ground() == false)
            {
                // 生成後一度しか呼ばれない ( 以後trueのまま )
                static_cast<Sphere_Vehicle*>(vehicle.get())->set_on_ground();
            }
        }

    }
}

// 壁へのレイキャスト    [ entity_managerに登録している "" プレイヤー、敵、乗り物 "" が処理されている ]
inline void ray_to_wall(
    const float elapsed_time,
    Entity_Manager& e_manager,
    Stage_Manager& s_manager,
    const vectors& vectors_)
{
    // プレイヤー
    Hit_Result result;
    DirectX::XMFLOAT3 wall_vec = {};
    /*for (auto index : std::get<0>(vectors_))
    {
        std::shared_ptr<Entity> player = e_manager.get_entity(index);
        if (ray_cast_wall(elapsed_time, player, s_manager, result, wall_vec))
        {
            const DirectX::XMFLOAT3 velocity = player->get_velocity() * elapsed_time;
            const DirectX::XMFLOAT3 position = player->get_position();
            player->set_position({ position.x - velocity.x, position.y, position.z - velocity.z });
            player->add_position({ wall_vec.x,0.0f,wall_vec.z });
            player->set_velocity({});
        }
    }*/

    // 敵
    wall_vec = {};
    for (auto index : std::get<1>(vectors_))
    {
        std::shared_ptr<Entity> enemy = e_manager.get_entity(index);
        result = {};
        if (ray_cast_wall(elapsed_time, enemy, s_manager, result, wall_vec))
        {
            const DirectX::XMFLOAT3 velocity = enemy->get_velocity() * elapsed_time;
            const DirectX::XMFLOAT3 position = enemy->get_position();
            enemy->set_position({ position.x - velocity.x, position.y, position.z - velocity.z });
            enemy->add_position({ wall_vec.x,0.0f,wall_vec.z });
            enemy->set_velocity({});
        }
    }

    // 乗り物
    wall_vec = {};
    for (auto index : std::get<2>(vectors_))
    {
        std::shared_ptr<Entity>  vehicle = e_manager.get_entity(index);
        const DirectX::XMFLOAT3& scale = vehicle->get_scale();

        result = {};
        //if (ray_cast_wall(elapsed_time, vehicle, s_manager, result, wall_vec))
        if (ray_cast_wall(elapsed_time, vehicle, scale, s_manager, result, wall_vec))
        {
            
            const DirectX::XMFLOAT3 velocity = vehicle->get_velocity() * elapsed_time;
            const DirectX::XMFLOAT3 position = vehicle->get_position();  //TODO: latest_position
            vehicle->set_position({ position.x - velocity.x, position.y, position.z - velocity.z });
            vehicle->add_position({ wall_vec.x,0.0f,wall_vec.z });

            //vehicle->set_position(result.position);

            if (velocity.y < 0.0f)
            {
                vehicle->set_friction(0.8f);
            }
        }

    }
}

// ステージ外にでているか[ entity_managerに登録している "" プレイヤー、敵、乗り物 "" が処理されている ]
inline void out_range(
    Entity_Manager& e_manager,
    const vectors& vectors_)
{

    // プレイヤー
    for (auto index : std::get<0>(vectors_))
    {
        judge_out_range(e_manager.get_entity(index), e_manager);
    }

    // 敵
    for (auto index : std::get<1>(vectors_))
    {
        judge_out_range(e_manager.get_entity(index), e_manager);

    }


    // 乗り物
    for (auto index : std::get<2>(vectors_))
    {
        judge_out_range(e_manager.get_entity(index), e_manager);

    }
}


inline void entity_collide(
    const float elapsed_time_,
    Entity_Manager& e_manager,
    const vectors& vectors_
)
{

    std::vector<short> entities = {};
    for (auto index : std::get<1>(vectors_))    // 敵
    {
        entities.emplace_back(index);
    }
    for (auto index : std::get<2>(vectors_))    // 乗り物
    {
        entities.emplace_back(index);
    }

    // 総当たり
    const int size = static_cast<int>(entities.size());
    for(int i = 0; i < size - 1; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            std::shared_ptr<Entity> entity_a = e_manager.get_entity(entities.at(i));
            std::shared_ptr<Entity> entity_b = e_manager.get_entity(entities.at(j));

            if (sphere_vs_sphere(entity_a,entity_b))
            {
                sphere_vs_sphere_extrusion(entity_a, entity_b, elapsed_time_);
            }

        }
    }


}


void Collision_Manager::judge(const float elapsed_time)
{
    Entity_Manager& e_manager = Entity_Manager::instance();
    Stage_Manager& s_manager = Stage_Manager::instance();

    const vec_type_short& vec_player_indices   = e_manager.get_entities(Tag::Player);
    const vec_type_short& vec_enemy_indices    = e_manager.get_entities(Tag::Enemy);
    const vec_type_short& vec_vehicle_indices  = e_manager.get_entities(Tag::Vehicle);

    const vectors vectors_ = {
        vec_player_indices,
        vec_enemy_indices,
        vec_vehicle_indices
    };

    // 壁へのレイキャスト
    ray_to_wall(
        elapsed_time,
        e_manager, s_manager,
        vectors_
    );

    //// 床へのレイキャスト
    //ray_to_floor(
    //    elapsed_time,
    //    e_manager, s_manager,
    //    vectors_
    //);


    //// entity同士で当たり判定 (球)
    //entity_collide(
    //    elapsed_time,
    //    e_manager,
    //    vectors_
    //);

    // ステージ外にでているか
    out_range(
        e_manager,
        vectors_
    );
}
