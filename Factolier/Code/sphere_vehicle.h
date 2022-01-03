#pragma once

#include "entity.h"

class Sphere_Vehicle : public Entity
{
public:
    Sphere_Vehicle(class Scene_Manager* ptr_scene_manager_);
    ~Sphere_Vehicle()                       override = default;

    virtual void init()                             override {};
    virtual void update(const float elapsed_time_)  override;


    // Getter関数 //
    _NODISCARD const bool& get_is_free() { return m_is_free; }

    // Setter関数 //
    void move_direction(const DirectX::XMFLOAT3& direction_);
    void set_is_free() { m_is_free = true; }

private:
    void update_velocity(const float elapsed_time_);
    void rotate(const float elapsed_time_);

private:
    bool m_is_free = false;                               // 乗り物の動きが自由か ( 束縛/操作されている = false , 自由 = true )
};
