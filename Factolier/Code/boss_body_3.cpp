
#include "boss_body_3.h"
#include "model_filepaths.h"
#include "transform.h"

Boss_Body_3::Boss_Body_3(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_, std::weak_ptr<Entity> wkp_bodies_1, std::weak_ptr<Entity> wkp_bodies_2) : Boss_Body(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_body3, target_position_)
{
    wkp_bodies[0] = wkp_bodies_1;
    wkp_bodies[1] = wkp_bodies_2;

    const DirectX::XMFLOAT3& under_body_position = wkp_bodies[1].lock()->get_position();

    set_position({ under_body_position.x,under_body_position.y + body_height ,under_body_position.z });
    get_transform()->Update();
}

void Boss_Body_3::update(const float elapsed_time_)
{

}
