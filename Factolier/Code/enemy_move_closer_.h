#pragma once

#include "enemy.h"
#include "transform.h"
#include "timer.h"
#include "base_move_phase.h"


class Enemy_Move_Closer_ final : public Enemy
{
public:
    Enemy_Move_Closer_(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Move_Closer_()override = default;

    void init() override {};
    void update(const float elapsed_time_) override;

private:
    static bool is_find_target(const Enemy_Move_Closer_& me);    // エリア内で敵を見つけたかどうか
    
    void init_in_area();                                        // 初期化
    void find_in_area();                                        // エリア内の探索位置を決定
    void look_in_area();                                        // 探索位置に向く

    bool stop_velocity()
    {
        m_velocity->set({ 0.0f,0.0f,0.0f });
        return true;
    }

    static inline void look_target(Enemy_Move_Closer_& me, const DirectX::XMFLOAT3& target_)
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
        const float radian = acosf(dot)/*回転量*/;
        

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

    std::unique_ptr<Move_Phase<Enemy_Move_Closer_>> m_move_phase = nullptr;

};
