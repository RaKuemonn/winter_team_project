#pragma once

#include <windows.h>
#include "math_xmfloat.h"


CONST LONG SCREEN_WIDTH{ 1920 };
CONST LONG SCREEN_HEIGHT{ 1080 };
CONST BOOL FULLSCREEN{ FALSE };
CONST LPWSTR APPLICATION_NAME{ L"Theme_Sphere" };


#define CAST_I(f) static_cast<int>(f)
#define CAST_F(i) static_cast<float>(i)


template <typename T>
inline void safe_delete(T*& p)
{
    if (p != nullptr)
    {
        delete (p);
        (p) = nullptr;
    }
}



// --- 乱数 --- //

inline float random_float(const float range_min_, const float range_max_)
{
    int range = static_cast<int>(range_max_ - range_min_);
    if (range == 0.0f) range = 1;

    return static_cast<float>(rand() % range) + range_min_;
}

inline float random_float(const DirectX::XMFLOAT2 range_)
{
    return random_float(range_.x,range_.y);
}

inline DirectX::XMFLOAT3 random_float3(const DirectX::XMFLOAT2 range_x_, const DirectX::XMFLOAT2 range_y_, const DirectX::XMFLOAT2 range_z_)
{
    return { random_float(range_x_),random_float(range_y_),random_float(range_z_) };
}

inline DirectX::XMFLOAT3 random_range_box(
    const DirectX::XMFLOAT3& box_position_,
    const DirectX::XMFLOAT3& box_size_)
{
    const DirectX::XMFLOAT3 half_size = box_size_ * 0.5f;

    return random_float3(
        { box_position_.x - half_size.x, box_position_.x + half_size.x },
        { box_position_.y - half_size.y, box_position_.y + half_size.y },
        { box_position_.z - half_size.z, box_position_.z + half_size.z }
    );
}

// ----ーー---- //


// --- 固定 --- //
inline float clamp_float(const float value_, const float min_, const float max_)
{
    if (min_ < value_)
    {
        return min_;
    }
    if (max_ > value_)
    {
        return max_;
    }

    return value_;
}

inline float clamp_float(const float value_, const DirectX::XMFLOAT2 size_)
{
    return clamp_float(value_, size_.x, size_.y);
}

inline DirectX::XMFLOAT3 clamp_float3(
    const DirectX::XMFLOAT3& float3_,
    const DirectX::XMFLOAT2 range_x_,
    const DirectX::XMFLOAT2 range_y_,
    const DirectX::XMFLOAT2 range_z_)
{
    return {
        clamp_float(float3_.x, range_x_.x,range_x_.y),
        clamp_float(float3_.y, range_y_.x,range_y_.y),
        clamp_float(float3_.z, range_z_.x,range_z_.y)
    };;
}

inline DirectX::XMFLOAT3 clamp_float3_box(
    const DirectX::XMFLOAT3& float3_,
    const DirectX::XMFLOAT3& box_position_,
    const DirectX::XMFLOAT3& box_size_)
{
    const DirectX::XMFLOAT3 half_size = box_size_ * 0.5f;

    return clamp_float3(float3_,
        { box_position_.x - half_size.x, box_position_.x + half_size.x },
        { box_position_.y - half_size.y, box_position_.y + half_size.y },
        { box_position_.z - half_size.z, box_position_.z + half_size.z }
    );
}

// ----ーー---- //