
#include "boss_body_1.h"
#include "model_filepaths.h"
#include "stage_manager.h"
#include "collision.h"
#include "transform.h"

Boss_Body_1::Boss_Body_1(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Boss_Body(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_body1, target_position_)
{
    constexpr DirectX::XMFLOAT3 xmf3 = { 0.0f, 10.0f, 0.0f };

    const DirectX::XMFLOAT3 start   = get_position() + xmf3;
    const DirectX::XMFLOAT3 end     = get_position() - xmf3;

    Hit_Result hit_result;
    if (Stage_Manager::instance().ray_cast(start, end, &hit_result))
    {
        set_position(hit_result.position);
    }

    get_transform()->Update();
}

void Boss_Body_1::update(const float elapsed_time_)
{
    DirectX::XMFLOAT4 quaternion;
    DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f), DirectX::XMConvertToRadians(90.0f) * elapsed_time_));
    add_quaternion(quaternion);

    get_transform()->Update();
}
