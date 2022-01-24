#pragma once

#include "DirectXMath.h"


#if 0

class transform final
{
public:
    transform() = default;
    ~transform() = default;
    transform(const transform& obj) = delete;
    transform(const transform&& obj) = delete;

    // 更新
    void Update()
    {
        // 更新があったかの判断
        if (was_transition() == false) return;


        // 正規化された回転行列
        DirectX::XMMATRIX rotate    = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(&m_next_quaternion)));

        DirectX::XMMATRIX scale     = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_next_scale));

        DirectX::XMMATRIX translate = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_next_position));


        DirectX::XMStoreFloat4x4(&m_matrix, scale * rotate * translate);
    }

    // Getter関数 //
    const auto& get_position()  const { return m_offset; }
    const auto& get_scale()     const { return m_next_scale; }
    //const auto& get_angle()     const { return m_euler; }
    const auto& get_matrix()    const { return m_matrix; }
    const auto& get_axis_x()    const { return m_axis_x; }
    const auto& get_axis_y()    const { return m_axis_y; }
    const auto& get_axis_z()    const { return m_axis_z; }


    // Setter関数 //
    void set_position(const DirectX::XMFLOAT3& position_)       { m_next_position = position_; }
    void set_scale(const DirectX::XMFLOAT3& scale_)             { m_next_scale = scale_; }
    void set_euler(const DirectX::XMFLOAT3& euler_)             { m_next_quaternion = convert_to_quaternion(euler_); }
    void set_quaternion(const DirectX::XMFLOAT4& quaternion_)   { m_next_quaternion = quaternion_; }


private:
    // 位置
    DirectX::XMFLOAT3 m_next_position = {};

    // スケール
    DirectX::XMFLOAT3 m_scale = {};
    DirectX::XMFLOAT3 m_next_scale = {};

    // 回転
    DirectX::XMFLOAT4 m_quaternion = {};
    DirectX::XMFLOAT4 m_next_quaternion = {};

    // 行列
    union
    {
        struct
        {
            DirectX::XMFLOAT3 m_axis_x; float m_14;
            DirectX::XMFLOAT3 m_axis_y; float m_24;
            DirectX::XMFLOAT3 m_axis_z; float m_34;
            DirectX::XMFLOAT3 m_offset; float m_44;
        };
        DirectX::XMFLOAT4X4 m_matrix = {
            1.0f,0.0f,0.0f,0.0f,
            0.0f,1.0f,0.0f,0.0f,
            0.0f,0.0f,1.0f,0.0f,
            0.0f,0.0f,0.0f,1.0f
        };
    };


private:
    bool was_transition()
    {
        const bool next_pos = was_transition_xmfloat3(m_next_position,m_offset);
        const bool next_sca = was_transition_xmfloat3(m_next_scale,m_scale);
        const bool next_qua = was_transition_quaternion(m_next_quaternion, m_quaternion);

        // いずれか更新があったら更新する
        return (next_pos || next_sca || next_qua);
    }


    static bool was_transition_xmfloat3(const DirectX::XMFLOAT3& next, const DirectX::XMFLOAT3& now)
    {
        return DirectX::XMVector3Equal(DirectX::XMLoadFloat3(&next), DirectX::XMLoadFloat3(&now));
    }
    static bool was_transition_quaternion(const DirectX::XMFLOAT4& next, const DirectX::XMFLOAT4& now)
    {
        return DirectX::XMQuaternionEqual(DirectX::XMLoadFloat4(&next), DirectX::XMLoadFloat4(&now));
    }

    [[nodiscard]] static const DirectX::XMFLOAT4& convert_to_quaternion(const DirectX::XMFLOAT3& euler_) 
    {
        DirectX::XMFLOAT4 quaternion;
    
        DirectX::XMStoreFloat4(&quaternion,
            DirectX::XMQuaternionRotationRollPitchYawFromVector(
                DirectX::XMLoadFloat3(&euler_)
            ));
    
        return quaternion;
    }
};

#else

#include "scale.h"
#include "rotate.h"
#include "position.h"


class Transform final
{
public:
    Transform()
    {
        m_was_transition = true;
        Update();
    };
    ~Transform() = default;
    Transform(const Transform& obj) = delete;
    Transform(const Transform&& obj) = delete;

    // 更新
    void Update()
    {
        // 更新があったかの判断
        if (m_was_transition == false) return;


        // 正規化された回転行列
        m_latest_quaternion = m_quaternion.get();
        m_latest_euler      = m_quaternion.get_euler();
        const DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionNormalize(DirectX::XMLoadFloat4(&m_latest_quaternion)));

        // スケール
        m_latest_scale = m_scale.get();
        const DirectX::XMMATRIX scale = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&m_latest_scale));

        // 位置
        const DirectX::XMMATRIX translate = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&m_position.get()));

        
        // 姿勢行列を更新
        DirectX::XMStoreFloat4x4(&m_matrix, scale * rotate * translate);


        // リセット
        m_was_transition = false;
    }

    // Getter関数 //

    _NODISCARD const auto  get_was_tansition()      const { return m_was_transition; }
    _NODISCARD const auto& get_latest_position()    const { return m_position.get(); }  // 最新のposition
    _NODISCARD const auto& get_position()           const { return m_offset; }
    _NODISCARD const auto& get_scale()              const { return m_latest_scale; }
    _NODISCARD const auto& get_quaternion()         const { return m_latest_quaternion; }
    _NODISCARD const auto& get_euler()              const { return m_latest_euler; }
    _NODISCARD const auto& get_matrix()             const { return m_matrix; }
    _NODISCARD const auto& get_axis_x()             const { return m_axis_x; }
    _NODISCARD const auto& get_axis_y()             const { return m_axis_y; }
    _NODISCARD const auto& get_axis_z()             const { return m_axis_z; }

    static _NODISCARD const DirectX::XMFLOAT4& quaternion_default()
    {
        return { 0.0f,0.0f,0.0f,1.0f };
    }



    // Setter関数 //  /* このsetterを呼んでもはupdateを通らないと姿勢更新しない */
    void set_position(const DirectX::XMFLOAT3& position_)                                   { if(same_check(position_,m_position.get()))return;        m_position.set(position_);                        m_was_transition = true; }
    void set_scale(const DirectX::XMFLOAT3& scale_)                                         { if(same_check(scale_,m_scale.get()))return;              m_scale.set(scale_);                              m_was_transition = true; }
    void set_euler(const DirectX::XMFLOAT3& euler_)                                         { if(same_check(euler_,m_quaternion.get_euler()))return;   m_quaternion.set_euler(euler_);                   m_was_transition = true; }
    void set_quaternion(const DirectX::XMFLOAT4& quaternion_)                               { if(zero_check(quaternion_))return;                              m_quaternion.set(quaternion_);                    m_was_transition = true; }
    void set_slerp_quaternion(const DirectX::XMFLOAT4& to_quaternion_, const float ratio)   { if(zero_check(to_quaternion_))return;                           m_quaternion.set_slerp(to_quaternion_, ratio);    m_was_transition = true; }
    //void add_euler(const DirectX::XMFLOAT3& add_euler_)                                     { zero_check(add_euler_);       m_quaternion.add_euler(add_euler_);                   m_was_transition = true; }
    void add_position(const DirectX::XMFLOAT3& add_position_)                               { if(zero_check(add_position_))return;                            m_position.add(add_position_);                    m_was_transition = true; }
    void add_scale(const DirectX::XMFLOAT3& add_scale_)                                     { if(zero_check(add_scale_))return;                               m_scale.add(add_scale_);                          m_was_transition = true; }
    void add_quaternion(const DirectX::XMFLOAT4& add_quaternion_)                           { if(zero_check(add_quaternion_))return;                          m_quaternion.add(add_quaternion_);                m_was_transition = true; }
    

private:
    // 更新があったかのフラグ
    bool m_was_transition = false;


    // 位置
    Position m_position;

    // スケール
    Scale m_scale;
    DirectX::XMFLOAT3 m_latest_scale = {};      // 更新した最新のスケール値

    // 回転
    Rotate m_quaternion;
    DirectX::XMFLOAT4 m_latest_quaternion = {}; // 更新した最新のクオータニオン回転値
    DirectX::XMFLOAT3 m_latest_euler = {};      // 更新した最新のオイラー回転値


    // 行列
    union
    {
        struct
        {
            DirectX::XMFLOAT3 m_axis_x; float m_14;
            DirectX::XMFLOAT3 m_axis_y; float m_24;
            DirectX::XMFLOAT3 m_axis_z; float m_34;
            DirectX::XMFLOAT3 m_offset; float m_44;
        };
        DirectX::XMFLOAT4X4 m_matrix;
    };

private:
    static bool zero_check(const DirectX::XMFLOAT3& f3)
    {
        return DirectX::XMVector3Equal(DirectX::XMLoadFloat3(&f3), DirectX::XMVectorZero());
    }
    static bool zero_check(const DirectX::XMFLOAT4& f4)
    {
        return DirectX::XMVector4Equal(DirectX::XMLoadFloat4(&f4), DirectX::XMVectorZero());
    }

    static bool same_check(const DirectX::XMFLOAT3& f3, const DirectX::XMFLOAT3& f3_)
    {
        return DirectX::XMVector3Equal(DirectX::XMLoadFloat3(&f3), DirectX::XMLoadFloat3(&f3_));
    }

    static bool same_check(const DirectX::XMFLOAT4& f4, const DirectX::XMFLOAT4& f4_)
    {
        return DirectX::XMVector4Equal(DirectX::XMLoadFloat4(&f4), DirectX::XMLoadFloat4(&f4_));
    }

};


#endif
