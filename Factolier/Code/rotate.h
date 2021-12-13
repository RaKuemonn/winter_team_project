#pragma once

#include "math_xmfloat.h"
#include <valarray>

class rotate
{
public:
    rotate() = default;
    ~rotate() = default;

    const auto& operator *= (const DirectX::XMFLOAT4& quaternion_) const
    {
        DirectX::XMFLOAT4 rotated_quaternion = {};
        DirectX::XMStoreFloat4(&rotated_quaternion,
            DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
                DirectX::XMLoadFloat4(&m_quaternion),
                DirectX::XMLoadFloat4(&quaternion_)
            ))
        );
        return rotated_quaternion;
    }
    const auto& operator *= (const DirectX::XMFLOAT3& add_euler_) const
    {
        DirectX::XMFLOAT4 rotated_quaternion = {};
        DirectX::XMStoreFloat4(&rotated_quaternion,
            DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
                DirectX::XMLoadFloat4(&m_quaternion),
                DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMVectorSet(add_euler_.x, add_euler_.z, add_euler_.y, 0.0f)))
            ))
        );
        return rotated_quaternion;
    }

    // Getter関数 //
    const auto& get() const       { return m_quaternion; }
    const auto& get_euler() const { return convert_to_euler(m_quaternion); }

    // Setter関数 //
    void set(const DirectX::XMFLOAT4& quaternion_)  { m_quaternion = quaternion_; }
    void set_euler(const DirectX::XMFLOAT3& euler_) { DirectX::XMStoreFloat4(&m_quaternion,DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&euler_)))); }
    void set_slerp(const DirectX::XMFLOAT4& to_quaternion, const float ratio)
    {
        DirectX::XMStoreFloat4(&m_quaternion, DirectX::XMQuaternionSlerp(
            DirectX::XMLoadFloat4(&m_quaternion),
            DirectX::XMQuaternionMultiply(DirectX::XMLoadFloat4(&m_quaternion), DirectX::XMLoadFloat4(&to_quaternion)),
            ratio)
        );
    }

    void add(const DirectX::XMFLOAT4& quaternion_)
    {
        DirectX::XMStoreFloat4(&m_quaternion,
            DirectX::XMQuaternionMultiply(
                DirectX::XMLoadFloat4(&m_quaternion),
                DirectX::XMLoadFloat4(&quaternion_)
            )
        );
    }

    //void add_euler(const DirectX::XMFLOAT3& add_euler_)
    //{
    //    
    //    const DirectX::XMVECTOR euler_quaternion    = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMVectorSet(add_euler_.x, add_euler_.y, add_euler_.z, 0.0f)));
    //    
    //    
    //    DirectX::XMStoreFloat4(&m_quaternion,
    //        DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
    //            DirectX::XMLoadFloat4(&m_quaternion),
    //            euler_quaternion
    //        ))
    //    );
    //
    //}


private:
    DirectX::XMFLOAT4 m_quaternion = {0.0f,0.0f,0.0f,1.0f};

private:

    // 多分これでクオータニオンからオイラーに変換できる
    static const DirectX::XMFLOAT3& convert_to_euler(const DirectX::XMFLOAT4& q)
    {
        const auto sy = 2 * q.x * q.z + 2 * q.y * q.w;
        const auto unlocked = std::abs(sy) < 0.99999f;

        
        DirectX::XMFLOAT3 euler =
        {
            unlocked ? std::atan2(-(2 * q.y * q.z - 2 * q.x * q.w), 2 * q.w * q.w + 2 * q.z * q.z - 1)
            : std::atan2(2 * q.y * q.z + 2 * q.x * q.w, 2 * q.w * q.w + 2 * q.y * q.y - 1),

            std::asin(sy),

            unlocked ? std::atan2(-(2 * q.x * q.y - 2 * q.z * q.w), 2 * q.w * q.w + 2 * q.x * q.x - 1)
            : 0
        };

        return euler;
    }
};