#pragma once

#include "enemy.h"
#include "transform.h"
#include "timer.h"

class Enemy_Move_Closer final : public Enemy
{
public:
    Enemy_Move_Closer(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Move_Closer()override = default;

    void init() override {};
    void update(const float elapsed_time_) override;

private:
    //static bool is_search_to_area();
    static bool is_find_target(const Enemy_Move_Closer& me);    // エリア内で敵を見つけたかどうか

    void move_phase(const float elapsed_time_);

    void search_in_area(const float elapsed_time_);             // エリア内の探索
    void init_in_area(const float elased_time_);
    void find_in_area(const float elapsed_time_);
    void move_in_area(const float elapsed_time_);
    void look_in_area(const float elapsed_time_);
    void idle_in_area(const float elapsed_time_);


    void seek_to_target(const float elapsed_time_);             // 目標を見つけた
    void init_to_target(const float elapsed_time_);             // 初期化
    void look_to_target(const float elapsed_time_);             // 目標方向に向きを変える
    void move_to_target(const float elapsed_time_);             // 目標に向かって移動する　・　３秒継続

    static inline void look_target(Enemy_Move_Closer& me, const DirectX::XMFLOAT3& target_, const float elapsed_time_)
    {
        const DirectX::XMFLOAT3 axis_x = me.get_axis_x();
        const DirectX::XMVECTOR right_direction = DirectX::XMLoadFloat3(&axis_x);

        const DirectX::XMFLOAT3 axis_z = me.get_axis_z();
        const DirectX::XMVECTOR front_direction = DirectX::XMLoadFloat3(&axis_z);

        const DirectX::XMFLOAT3 target_position = { target_.x,0.0f,target_.z };
        const DirectX::XMFLOAT3 position = { me.get_position().x,0.0f,me.get_position().z };

        const DirectX::XMVECTOR to_target_direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&target_position), DirectX::XMLoadFloat3(&position)));


        const float dot_right_or_left = DirectX::XMVectorGetX(DirectX::XMVector3Dot(to_target_direction, right_direction));
        // sign = 符号     +1.0f か - 1.0f のどちらかが入る
        const float sign = (dot_right_or_left > 0.0f) ? +1.0f : -1.0f;
        const float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(to_target_direction, front_direction));           // 正規化されたベクトルによる内積

        if (dot >= 1.0f - FLT_EPSILON) return;


        const DirectX::XMVECTOR cross = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(front_direction, to_target_direction));   // 正規化されたベクトルによる外積
        const float radian = elapsed_time_ == 0.0f ?
                                acosf(dot)/*回転量*/ /*回転方向*/ :
                                acosf(dot)/*回転量*/ /*回転方向*/ * 2.0f * elapsed_time_/*フレームレート*/;

        // TODO : 今は回転速度が一定 , 補間して自然にする

        const DirectX::XMVECTOR xmvec_quaternion = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationAxis(cross, radian));
        DirectX::XMFLOAT4 xmf4_quaternion;
        DirectX::XMStoreFloat4(&xmf4_quaternion, xmvec_quaternion);

        // 回転させる
        me.get_transform()->add_quaternion(xmf4_quaternion);
    }

private:
    Timer m_timer;
    DirectX::XMFLOAT3 m_area_target_position    = {};
    DirectX::XMFLOAT3 m_area_origin_position    = {};
    DirectX::XMFLOAT3 m_area_size               = {};
    bool m_is_find_target       = false;
    bool m_is_move_phase_init   = true;
};
