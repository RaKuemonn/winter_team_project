#pragma once

#include "math_xmfloat.h"



class Velocity final
{
public:
    Velocity() = default;
    ~Velocity() = default;

    void update(const float elapsed_time)
    {
        update_friction(*this);
        update_air_drag(*this, /* 空気抵抗率 */ 0.1f);

        m_velocity += (m_resultant / m_mass) * elapsed_time; // フレーム単位の加速力を加える

        epsilon_cut(*this); // 閾値以下の切り捨て

        m_resultant = {};
    }

    // Getter関数 //
    _NODISCARD const auto& get() const                  { return m_velocity; }

    // Setter関数 //
    void set(const DirectX::XMFLOAT3& velocity_)        { m_velocity = velocity_; }
    void set_x(const float velocity_x_)                 { m_velocity.x = velocity_x_; }
    void set_y(const float velocity_y_)                 { m_velocity.y = velocity_y_; }
    void set_z(const float velocity_z_)                 { m_velocity.z = velocity_z_; }
    void add(const DirectX::XMFLOAT3& add_velocity_)    { m_resultant += add_velocity_; }     // ここで加速した値はupdate()を通るまで、加速した結果のm_velocityは得られない
    void set_friction(const float friction_)            { m_friction = friction_; }
    void set_mass(const float mass_)                    { m_mass = mass_; }


private:
    DirectX::XMFLOAT3 m_velocity    = {};   // 現在の速力
    DirectX::XMFLOAT3 m_resultant   = {};   // フレーム間で加えられた合力
    float             m_friction    = {};   // 摩擦力
    float             m_mass        = {};   // 質量

    static constexpr float gravity  = -9.8f;

private:
    // 摩擦処理
    inline static void update_friction(Velocity& me)
    {
        // m_massとm_frictionの初期値が0なので、
        // set_mass()とset_friction()しないと計算上摩擦処理は動かない
        const DirectX::XMFLOAT3 friction = me.m_velocity * -1.0f * (me.m_mass * -gravity) * me.m_friction;
        me.add(friction);
    }

    // 空気抵抗処理
    inline static void update_air_drag(Velocity& me, const float air_drag_)
    {
        me.add(me.m_velocity * -1.0f * air_drag_);
    }

    // 閾値の切り捨て処理
    inline static void epsilon_cut(Velocity& me)
    {
        // 速度が閾値以下のとき,値を切り捨てする
        if (DirectX::XMVectorGetX(
            DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&me.m_velocity))) <= FLT_EPSILON
            )
        {
            me.m_velocity = {};
        }
    }
};