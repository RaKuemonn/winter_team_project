#pragma once

#include <functional>
#include "base_move.h"
#include "math_xmfloat.h"

template<typename Enemy>
class Target_Mover : public Base_Move<Enemy>
{
public:
    void update(float elapsed_time_, Enemy& type_) override
    {
        look_target(type_, type_.get_target_position(), elapsed_time_);
        
        move_front(type_);
    }

private:
    static void move_front(Enemy& type_)
    {
        constexpr float speed = 3.5f;
        const DirectX::XMFLOAT3& front_dir = type_.get_axis_z();
        type_.add_velocity({ front_dir.x * speed, front_dir.y * speed, front_dir.z * speed });
    }

    static void look_target(Enemy& me, const DirectX::XMFLOAT3& target_, const float elapsed_time_)
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
        me.add_quaternion(xmf4_quaternion);
    }

    
};
