#pragma once

#include "math_xmfloat.h"
#include <valarray>


class Rotate final
{
public:
    Rotate() = default;
    ~Rotate() = default;

    const auto& operator *= (const Rotate& quaternion_)
    {
        DirectX::XMStoreFloat4(&m_quaternion,
            DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
                DirectX::XMLoadFloat4(&m_quaternion),
                DirectX::XMLoadFloat4(&quaternion_.get())
            ))
        );
        return this;
    }
    const auto& operator *= (const DirectX::XMFLOAT4& quaternion_)
    {
        DirectX::XMStoreFloat4(&m_quaternion,
            DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
                DirectX::XMLoadFloat4(&m_quaternion),
                DirectX::XMLoadFloat4(&quaternion_)
            ))
        );
        return this;
    }
    const auto& operator *= (const DirectX::XMFLOAT3& add_euler_)
    {
        DirectX::XMStoreFloat4(&m_quaternion,
            DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
                DirectX::XMLoadFloat4(&m_quaternion),
                DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMVectorSet(add_euler_.x, add_euler_.z, add_euler_.y, 0.0f)))
            ))
        );
        return this;
    }

    // Getter?֐? //
    _NODISCARD const DirectX::XMFLOAT4& get() const       { return m_quaternion; }
    _NODISCARD const DirectX::XMFLOAT3  get_euler() const { return convert_to_euler(m_quaternion); }

    // Setter?֐? //
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
            DirectX::XMQuaternionNormalize(DirectX::XMQuaternionMultiply(
                DirectX::XMLoadFloat4(&m_quaternion),
                DirectX::XMLoadFloat4(&quaternion_)
            ))
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

    // ?????????ŃN?I?[?^?j?I???????I?C???[?ɕϊ??ł??? ?????ӂɊp?x?????܂??Ȃ??ꍇ?????? (?l??-nan(ind)?????邱?Ƃ?????)
    static const DirectX::XMFLOAT3 convert_to_euler(const DirectX::XMFLOAT4& q)
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

        //DirectX::XMFLOAT3 euler = {};
        /*double q0q0 = static_cast<double>(q.x) * static_cast<double>(q.x);
        double q0q1 = static_cast<double>(q.x) * static_cast<double>(q.y);
        double q0q2 = static_cast<double>(q.x) * static_cast<double>(q.z);
        double q0q3 = static_cast<double>(q.x) * static_cast<double>(q.w);
        double q1q1 = static_cast<double>(q.y) * static_cast<double>(q.y);
        double q1q2 = static_cast<double>(q.y) * static_cast<double>(q.z);
        double q1q3 = static_cast<double>(q.y) * static_cast<double>(q.w);
        double q2q2 = static_cast<double>(q.z) * static_cast<double>(q.z);
        double q2q3 = static_cast<double>(q.z) * static_cast<double>(q.w);
        double q3q3 = static_cast<double>(q.w) * static_cast<double>(q.w);
        euler.x = std::atan2(2.0 * (q2q3 + q0q1), q0q0 - q1q1 - q2q2 + q3q3);
        euler.y = std::asin(2.0 * (q0q2 - q1q3));
        euler.z = std::atan2(2.0 * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3);*/

        /*DirectX::XMFLOAT4X4 matrix;
        DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&q)));
        euler.x = std::atan2(matrix._32, matrix._33);
        euler.y = std::asin(-matrix._31);
        euler.z = std::atan2(matrix._21, matrix._11);*/


        ////?@TO_DO: ?e?X?g
        //if (isnan(euler.y))
        //{
        //    int a = 0;
        //    a = 1;
        //}

        return euler;
    }
};