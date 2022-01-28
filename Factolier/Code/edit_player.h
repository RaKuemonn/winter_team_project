#pragma once

#include "entity.h"


class Edit_Player final : public Entity
{
public:
    Edit_Player(class Scene_Manager* ptr_scene_manager_);
    ~Edit_Player()                               override = default;

    void init()                             override;
    void update(const float elapsed_time_)  override;

    void set_clear(){};

private:

private:
    DirectX::XMFLOAT3 input_direction = {};
};
