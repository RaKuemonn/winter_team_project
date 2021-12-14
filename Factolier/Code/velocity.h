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
        update_air_drag(elapsed_time, *this, /* ��C��R�� */ 0.5f);

        m_velocity += (m_impulse / m_mass) * elapsed_time;

        m_impulse = {};
    }

    // Getter�֐� //
    _NODISCARD const auto& get() const                  { return m_velocity; }

    // Setter�֐� //
    void set(const DirectX::XMFLOAT3& velocity_)        { m_velocity = velocity_; }
    void add(const DirectX::XMFLOAT3& add_velocity_)    { m_impulse += add_velocity_; }     // �����ŉ��������l��update()��ʂ�܂ŁA�����������ʂ�m_velocity�͓����Ȃ�
    void set_friction(const float friction_)            { m_friction = friction_; }
    void set_mass(const float mass_)                    { m_mass = mass_; }


private:
    DirectX::XMFLOAT3 m_velocity    = {};   // ���݂̑���
    DirectX::XMFLOAT3 m_impulse     = {};   // �t���[���P�ʂ̉�����
    float             m_friction    = {};   // ���C��
    float             m_mass        = {};   // ����

    static constexpr float gravity  = -9.8f;

private:
    // ���C����
    static void update_friction(const float elapsed_time, Velocity& me)
    {
        // m_mass��m_friction�̏����l��0�Ȃ̂ŁA
        // set_mass()��set_friction()���Ȃ��ƌv�Z�㓮���Ȃ�
        me.add(me.m_velocity * -1.0f * (me.m_mass * -gravity) * me.m_friction);
    }

    // ��C��R����
    static void update_air_drag(const float elapsed_time, Velocity& me, const float air_drag_)
    {
        me.add(me.m_velocity * -1.0f * air_drag_);
    }
};