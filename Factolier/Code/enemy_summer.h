#pragma once

#include "enemy_move_closer_.h"


class Enemy_Summer final : public Enemy_Move_Closer_
{
public:
    Enemy_Summer(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Summer() override = default;

    void init() override {};
    void update(const float elapsed_time_) override;
};