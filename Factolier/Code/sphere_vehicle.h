#pragma once

#include "entity.h"

constexpr float SPHERE_SCALE_DECREASE = -0.25f;

class Sphere_Vehicle : public Entity
{
public:
    Sphere_Vehicle(class Scene_Manager* ptr_scene_manager_, const char* filename_);
    ~Sphere_Vehicle()                       override = default;

    virtual void init()                             override {};
    virtual void update(const float elapsed_time_)  override;


    // GetterŠÖ” //
    _NODISCARD const bool& get_is_free() { return m_is_free; }
    _NODISCARD const bool& get_on_ground() { return m_on_ground; }

    // SetterŠÖ” //
    void move_direction(const DirectX::XMFLOAT3& direction_);
    void set_is_free() { m_is_free = true; }
    void set_on_ground() { m_on_ground = true; }        // collision_manager‚Å¶¬Œãˆê“x‚µ‚©ŒÄ‚Î‚ê‚È‚¢‚æ‚¤‚Éˆ—‚ğ‘‚¢‚Ä‚¢‚é

private:
    void update_velocity(const float elapsed_time_);
    void rotate(const float elapsed_time_);

private:
    bool m_is_free = false;                               // æ‚è•¨‚Ì“®‚«‚ª©—R‚© ( ‘©”›/‘€ì‚³‚ê‚Ä‚¢‚é = false , ©—R = true )
    bool m_on_ground = false;
    bool m_is_jump = false;
    //std::unique_ptr<class Timer> m_timer = nullptr;

};
