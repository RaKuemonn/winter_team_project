#pragma once

#include "entity.h"
#include "velocity.h"


class Enemy final : public Entity
{
public:
    Enemy(class Scene_Manager* ptr_scene_manager_);
    ~Enemy()                                override = default;

    void init()                             override {};
    void update(const float elapsed_time_)  override;
    void render()                           override;

private:
    std::unique_ptr<Velocity> wkp_vehicle;
    DirectX::XMFLOAT3 input_direction = {};

};

