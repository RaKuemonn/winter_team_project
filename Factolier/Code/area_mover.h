#pragma once

#include <functional>
#include "base_move.h"
#include "math_xmfloat.h"

template<typename Enemy>
class Area_Mover : public Base_Move<Enemy>
{
public:
    void update(float elapsed_time_, Enemy& type_) override
    {
        move_front(type_);
    }

private:
    static void move_front(Enemy& type_)
    {
        constexpr float speed = 3.5f;
        const DirectX::XMFLOAT3& front_dir = type_.get_axis_z();
        type_.add_velocity({ front_dir.x * speed, front_dir.y * speed, front_dir.z * speed });
    }

private:
    float timer = 0.0f;
    
};
