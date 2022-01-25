#pragma once

#include "boss_body.h"
#include "Timer.h"

class Boss_Body_3 final : public Boss_Body
{
public:
    Boss_Body_3(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_, std::weak_ptr<Entity> wkp_bodies_1, std::weak_ptr<Entity> wkp_bodies_2);
    ~Boss_Body_3()override = default;

    void init() override {};
    void update(const float elapsed_time_) override;

private:
    void check_reference(const float elapsed_time_);

private:
    static constexpr int body_size = 2;
    Timer m_timer;

    std::weak_ptr<Entity> wkp_bodies[body_size] = {};
};

