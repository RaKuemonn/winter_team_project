
#include "enemy_fall.h"
#include "model_filepaths.h"

// TODO: ƒ‚ƒfƒ‹Žw’è
Enemy_Fall::Enemy_Fall(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy_Move_Closer_(ptr_scene_manager_, Model_Paths::Entity::enemy_move_fall, target_position_)
{

}

void Enemy_Fall::update(const float elapsed_time_)
{
    Enemy_Move_Closer_::update(elapsed_time_);
}
