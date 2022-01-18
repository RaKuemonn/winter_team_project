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
        constexpr float speed = 1.5f;
        type_.add_velocity(type_.get_axis_z() * speed);
    }

private:
    float timer = 0.0f;
    
};
