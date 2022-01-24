#pragma once

#include "enemy_move_closer_.h"


class Enemy_Winter final : public Enemy_Move_Closer_
{
public:
    Enemy_Winter(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Winter() override = default;

    void init() override {};
    void update(const float elapsed_time_) override;
};