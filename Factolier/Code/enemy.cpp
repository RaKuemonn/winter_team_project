
#include "enemy.h"
#include "transform.h"
#include "scene_manager.h"


Enemy::Enemy(Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_) : target_position(target_position_)
{

    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model(filename_));
    set_tag(Tag::Enemy);

}

void Enemy::update_velocity(const float elapsed_time_)
{
    constexpr DirectX::XMFLOAT3 gravity = { 0.0f,-3.0f * 9.8f,0.0f };
    m_velocity->add(gravity);

    // ‘¬“x‚ÌXV
    m_velocity->update(elapsed_time_);
}