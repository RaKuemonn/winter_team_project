#pragma once

#include "entity.h"
#include "velocity.h"

class Sphere_Vehicle : public Entity
{
public:
    Sphere_Vehicle(class Scene_Manager* ptr_scene_manager_);
    ~Sphere_Vehicle()                       override = default;

    virtual void init()                             override {};
    virtual void update(const float elapsed_time_)  override;
    virtual void render()                           override;


    // GetterŠÖ” //
    _NODISCARD const bool get_is_free() { return m_is_free; }

    // SetterŠÖ” //
    void move_direction(const DirectX::XMFLOAT3& direction_);

private:
    void update_velocity(const float elapsed_time_);
    void rotate(const float elapsed_time_);
    void collision();
    //inline static void input(DirectX::XMFLOAT3& input_direction_, Input_Manager& input_);

private:
    std::unique_ptr<Velocity> m_velocity = nullptr;
    bool m_is_free = false;                               // æ‚è•¨‚Ì“®‚«‚ª©—R‚© ( ‘©”›/‘€ì‚³‚ê‚Ä‚¢‚é = false , ©—R = true )
    //DirectX::XMFLOAT3 input_direction = {};
};
