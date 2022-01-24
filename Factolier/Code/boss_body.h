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
    Boss_Body(class Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_, const short hp_);
    ~Boss_Body()override = default;

    virtual void init() override = 0;
    virtual void update(const float elapsed_time_) override = 0;
    
    _NODISCARD const bool& get_was_died() const { return m_was_died; }
    void set_was_died() { m_was_died = true; }

protected:
    void update_collision_detection();
    bool check_im_hp();                         // ‘Ì—Í‚ª0‚É‚È‚Á‚½‚ç m_was_died‚ªtrue‚É‚È‚é

    float m_euler_y                             = 0.0f;

private:
    bool m_was_died                             = false;
    static constexpr int collide_array_size     = 8;
    std::weak_ptr<class Collide_Detection> wkp_collide_detection[collide_array_size];
};

