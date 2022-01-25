
#include "boss_body_1.h"
#include "model_filepaths.h"
#include "stage_manager.h"
#include "collision.h"
#include "transform.h"

constexpr int body_1_hp = 2;

Boss_Body_1::Boss_Body_1(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Boss_Body(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_body1, target_position_, body_1_hp)
{
    constexpr DirectX::XMFLOAT3 xmf3 = { 0.0f, 20.0f, 0.0f };
    constexpr DirectX::XMFLOAT3 zero = { 0.0f, 0.0f, 0.0f };

    const DirectX::XMFLOAT3 start   = zero + xmf3;
    const DirectX::XMFLOAT3 end     = zero - xmf3;

    Hit_Result hit_result;
    if (Stage_Manager::instance().ray_cast(start, end, &hit_result))
    {
        set_position(hit_result.position);
    }

    get_transform()->Update();
}

void Boss_Body_1::update(const float elapsed_time_)
{
    //DirectX::XMFLOAT4 quaternion;
    //DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMConvertToRadians(90.0f) * elapsed_time_));
    //add_quaternion(quaternion);

    if(check_im_hp() == false)
    {
        constexpr float rotate_speed = DirectX::XMConvertToRadians(30.0f);
        m_euler_y += rotate_speed * elapsed_time_;

        constexpr float pi = DirectX::XMConvertToRadians(180.0f);
        if(m_euler_y < pi)
        {
            m_euler_y += -2.0f * pi;
        }

        // 回転処理
        DirectX::XMFLOAT4 quaternion;
        DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), m_euler_y));
        set_quaternion(quaternion);
    }

    // 当たり判定位置の更新
    update_collision_detection();
    

    // 姿勢の更新
    get_transform()->Update();
}
