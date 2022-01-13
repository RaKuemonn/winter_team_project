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
        update_air_drag(*this, /* ��C��R�� */ 0.1f);

        m_velocity += (m_resultant / m_mass) * elapsed_time; // �t���[���P�ʂ̉����͂�������

        epsilon_cut(*this); // 臒l�ȉ��̐؂�̂�

        m_resultant = {};
    }

    // Getter�֐� //
    _NODISCARD const auto& get() const                  { return m_velocity; }

    // Setter�֐� //
    void set(const DirectX::XMFLOAT3& velocity_)        { m_velocity = velocity_; }
    void set_x(const float velocity_x_)                 { m_velocity.x = velocity_x_; }
    void set_y(const float velocity_y_)                 { m_velocity.y = velocity_y_; }
    void set_z(const float velocity_z_)                 { m_velocity.z = velocity_z_; }
    void add(const DirectX::XMFLOAT3& add_velocity_)    { m_resultant += add_velocity_; }     // �����ŉ��������l��update()��ʂ�܂ŁA�����������ʂ�m_velocity�͓����Ȃ�
    void set_friction(const float friction_)            { m_friction = friction_; }
    void set_mass(const float mass_)                    { m_mass = mass_; }


private:
    DirectX::XMFLOAT3 m_velocity    = {};   // ���݂̑���
    DirectX::XMFLOAT3 m_resultant   = {};   // �t���[���Ԃŉ�����ꂽ����
    float             m_friction    = {};   // ���C��
    float             m_mass        = {};   // ����

    static constexpr float gravity  = -9.8f;

private:
    // ���C����
    inline static void update_friction(Velocity& me)
    {
        // m_mass��m_friction�̏����l��0�Ȃ̂ŁA
        // set_mass()��set_friction()���Ȃ��ƌv�Z�㖀�C�����͓����Ȃ�
        const DirectX::XMFLOAT3 friction = me.m_velocity * -1.0f * (me.m_mass * -gravity) * me.m_friction;
        me.add(friction);
    }

    // ��C��R����
    inline static void update_air_drag(Velocity& me, const float air_drag_)
    {
        me.add(me.m_velocity * -1.0f * air_drag_);
    }

    // 臒l�̐؂�̂ď���
    inline static void epsilon_cut(Velocity& me)
    {
        // ���x��臒l�ȉ��̂Ƃ�,�l��؂�̂Ă���
        if (DirectX::XMVectorGetX(
            DirectX::XMVector3LengthSq(DirectX::XMLoadFloat3(&me.m_velocity))) <= FLT_EPSILON
            )
        {
            me.m_velocity = {};
        }
    }
};