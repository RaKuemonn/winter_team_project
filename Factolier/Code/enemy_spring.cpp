
#include "enemy_spring.h"
#include "model_filepaths.h"

// TODO: ƒ‚ƒfƒ‹Žw’è
Enemy_Spring::Enemy_Spring(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy_Move_Closer_(ptr_scene_manager_,nullptr, target_position_ )
{

}

void Enemy_Spring::update(const float elapsed_time_)
{
    Enemy_Move_Closer_::update(elapsed_time_);
}
