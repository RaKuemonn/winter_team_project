#pragma once

#include "enemy.h"

static constexpr float body_height = 12.0f;

namespace body
{
    constexpr float scale = 0.5f;
};

class Boss_Body : public Enemy
{
public:
    Boss_Body(class Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_);
    ~Boss_Body()override = default;

    virtual void init() override = 0;
    virtual void update(const float elapsed_time_) override = 0;

public:
    _NODISCARD const bool& get_is_free() const { return m_is_free; }
    void set_is_free() { m_is_free = true; }

private:
    bool m_is_free = false;
};

