#pragma once

#include "entity.h"
#include "velocity.h"

class Sphere_Vehicle final : public Entity
{
public:
    Sphere_Vehicle(class Scene_Manager* ptr_scene_manager_);
    virtual ~Sphere_Vehicle()               override = default;

    void init()                             override {};
    void update(const float elapsed_time_)  override;
    void render()                           override;

private:
    void update_velocity(const float elapsed_time_);
    void collision();

private:
    std::unique_ptr<Velocity> m_velocity = nullptr;
    DirectX::XMFLOAT3 input_direction = {};
};
