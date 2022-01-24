#pragma once

#include "entity.h"

constexpr float SPHERE_SCALE_DECREASE = -0.15f;

// 重要 :　スケール値が""""半径""""!!!!!

class Sphere_Vehicle : public Entity
{
public:
    Sphere_Vehicle(class Scene_Manager* ptr_scene_manager_, const char* filename_);
    ~Sphere_Vehicle()                       override = default;

    virtual void init()                             override {};
    virtual void update(const float elapsed_time_)  override;


    // Getter関数 //
    _NODISCARD const bool& get_is_free() { return m_is_free; }
    _NODISCARD const bool& get_on_ground() { return m_on_ground; }

    // Setter関数 //
    void move_direction(const DirectX::XMFLOAT3& direction_);
    void set_is_free() { m_is_free = true; }
    void set_on_ground() { m_on_ground = true; }        // collision_managerで生成後一度しか呼ばれないように処理を書いている
    void set_out_ground() { m_on_ground = false; }        
    void set_no_jump() { m_on_ground = false; }                 // Scene_Gameでクリア後ジャンプできなくするため

private:
    void update_velocity(const float elapsed_time_);
    void rotate(const float elapsed_time_);

private:
    bool m_is_free = false;                               // 乗り物の動きが自由か ( 束縛/操作されている = false , 自由 = true )
    bool m_on_ground = false;
    bool m_is_jump = false;
    //std::unique_ptr<class Timer> m_timer = nullptr;

};
