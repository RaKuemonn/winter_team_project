#pragma once

#include "boss_body.h"
#include "timer.h"


class Boss_Body_1 final : public Boss_Body
{
public:
    Boss_Body_1(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Boss_Body_1()override = default;

    void init() override {};
    void update(const float elapsed_time_) override;

private:

private:
    //static constexpr int body_size = 0;
    Timer m_timer;

    //std::weak_ptr<Entity> wkp_bodies[body_size] = {};
};

