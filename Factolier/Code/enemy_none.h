#pragma once

#include "enemy.h"


class Enemy_None final : public Enemy
{
public:
    Enemy_None(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_None() override = default;

    void init() override{};
    void update(const float elapsed_time_) override;
};