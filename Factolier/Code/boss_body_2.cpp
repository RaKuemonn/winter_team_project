
#include "boss_body_2.h"
#include "model_filepaths.h"
#include "transform.h"

Boss_Body_2::Boss_Body_2(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_, std::weak_ptr<Entity> wkp_bodies_1) : Boss_Body(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_body2, target_position_)
{
    wkp_body = wkp_bodies_1;

    const DirectX::XMFLOAT3& under_body_position = wkp_body.lock()->get_position();

    set_position({ under_body_position.x,under_body_position.y + body_height ,under_body_position.z });
    get_transform()->Update();
}

void Boss_Body_2::update(const float elapsed_time_)
{
    DirectX::XMFLOAT4 quaternion;
    DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), -DirectX::XMConvertToRadians(90.0f) * elapsed_time_));
    add_quaternion(quaternion);

    get_transform()->Update();
}
