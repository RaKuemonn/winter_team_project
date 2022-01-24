#pragma once

#include "enemy_move_closer_.h"


class Enemy_Spring final : public Enemy_Move_Closer_
{
public:
    Enemy_Spring(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Spring() override = default;

    void init() override {};
    void update(const float elapsed_time_) override;
};