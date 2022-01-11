#pragma once

#include "enemy.h"

class Enemy_Move_Closer final : public Enemy
{
public:
    Enemy_Move_Closer(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Move_Closer()override = default;

    void init() override {};
    void update(const float elapsed_time_) override;
};
