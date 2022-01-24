
#include "enemy_summer.h"
#include "model_filepaths.h"

// TODO: ƒ‚ƒfƒ‹Žw’è
Enemy_Summer::Enemy_Summer(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy_Move_Closer_(ptr_scene_manager_, Model_Paths::Entity::enemy_move_summer, target_position_)
{

}

void Enemy_Summer::update(const float elapsed_time_)
{
    Enemy_Move_Closer_::update(elapsed_time_);
}
