#pragma once

#include "math_xmfloat.h"



class Velocity final
{
public:
    Velocity() = default;
    ~Velocity() = default;

    void update(const float elapsed_time)
    {
        update_friction(elapsed_time, *this);
        update_air_drag(elapsed_time, *this, /* 空気抵抗率 */ 0.5f);

        m_velocity += (m_impulse / m_mass) * elapsed_time;

        m_impulse = {};
    }

    // Getter関数 //
    _NODISCARD const auto& get() const                  { return m_velocity; }

    // Setter関数 //
    void set(const DirectX::XMFLOAT3& velocity_)        { m_velocity = velocity_; }
    void add(const DirectX::XMFLOAT3& add_velocity_)    { m_impulse += add_velocity_; }     // ここで加速した値はupdate()を通るまで、加速した結果のm_velocityは得られない
    void set_friction(const float friction_)            { m_friction = friction_; }
    void set_mass(const float mass_)                    { m_mass = mass_; }


private:
    DirectX::XMFLOAT3 m_velocity    = {};   // 現在の速力
    DirectX::XMFLOAT3 m_impulse     = {};   // フレーム単位の加速力
    float             m_friction    = {};   // 摩擦力
    float             m_mass        = {};   // 質量

    static constexpr float gravity  = -9.8f;

private:
    // 摩擦処理
    static void update_friction(const float elapsed_time, Velocity& me)
    {
        // m_massとm_frictionの初期値が0なので、
        // set_mass()とset_friction()しないと計算上動かない
        me.add(me.m_velocity * -1.0f * (me.m_mass * -gravity) * me.m_friction);
    }

    // 空気抵抗処理
    static void update_air_drag(const float elapsed_time, Velocity& me, const float air_drag_)
    {
        me.add(me.m_velocity * -1.0f * air_drag_);
    }
};