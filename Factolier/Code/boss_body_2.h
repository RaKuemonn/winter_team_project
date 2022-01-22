#pragma once

#include "boss_body.h"
#include "Timer.h"

class Boss_Body_2 final : public Boss_Body
{
public:
    Boss_Body_2(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_, std::weak_ptr<Entity> wkp_bodies_1);
    ~Boss_Body_2()override = default;

    void init() override {};
    void update(const float elapsed_time_) override;

private:

private:
    //static constexpr int body_size = 1;
    Timer m_timer;

    std::weak_ptr<Entity> wkp_body = {};
};

