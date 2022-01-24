
#include "boss_body_3.h"
#include "model_filepaths.h"
#include "transform.h"

constexpr int body_3_hp = 4;

Boss_Body_3::Boss_Body_3(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_, std::weak_ptr<Entity> wkp_bodies_1, std::weak_ptr<Entity> wkp_bodies_2) : Boss_Body(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_body3, target_position_, body_3_hp)
{
    wkp_bodies[0] = wkp_bodies_1;
    wkp_bodies[1] = wkp_bodies_2;

    const DirectX::XMFLOAT3& under_body_position = wkp_bodies[1].lock()->get_position();

    set_position({ under_body_position.x,under_body_position.y + body_height ,under_body_position.z });
    get_transform()->Update();
}

void Boss_Body_3::update(const float elapsed_time_)
{
    if (check_im_hp() == false)
    {
        constexpr float rotate_speed = DirectX::XMConvertToRadians(90.0f);
        //m_euler_y += rotate_speed * elapsed_time_;

        // 回転処理
        DirectX::XMFLOAT4 quaternion;
        DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), m_euler_y));
        add_quaternion(quaternion);
    }

    // 当たり判定位置の更新
    update_collision_detection();


    // 姿勢の更新
    get_transform()->Update();
}
