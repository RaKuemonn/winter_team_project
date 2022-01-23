
#include "stage_2_movement.h"
#include "scene_manager.h"
#include "model.h"
#include "collision.h"
#include "imgui.h"
#include "transform.h"
#include "model_filepaths.h"


Stage_2_Movement::Stage_2_Movement(Scene_Manager* ptr_scene_manager_)
{
    load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Stage::stage_2_propeller, true));

    get_transform()->set_scale(stage::scale);
    get_transform()->set_euler(m_euler);
    get_transform()->set_position({ 0.0f,-3.0f,50.0f });
    get_transform()->Update();
}

void Stage_2_Movement::update(const float elapsed_time)
{
    old_transform = get_transform()->get_matrix();
    old_euler = m_euler;

    constexpr float pi = DirectX::XMConvertToRadians(90.0f);
    constexpr float half_pi = DirectX::XMConvertToRadians(45.0f);
    const float rotate_speed = half_pi * elapsed_time;
    m_euler.y += rotate_speed;
    if (m_euler.y >= 2.0f * pi)
    {
        m_euler.y += -4.0f * pi;
    }


    DirectX::XMFLOAT4 quaternion;
    DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&m_euler))));
    get_transform()->set_quaternion(quaternion);

    get_transform()->Update();
}




bool Stage_2_Movement::ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)
{

    // 前回のワールド行列で逆行列を算出
    DirectX::XMMATRIX WorldOldTransform = DirectX::XMLoadFloat4x4(&old_transform);
    DirectX::XMMATRIX InverseWorldOldTransform = DirectX::XMMatrixInverse(nullptr, WorldOldTransform);

    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);

    // ワールド空間の始点と終点をローカル空間上に変換
    DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldOldTransform);
    DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldOldTransform);
    // ローカル空間のレイの始点と終点
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, Start);
    DirectX::XMStoreFloat3(&localEnd, End);

    // 単位行列
    constexpr DirectX::XMFLOAT4X4 unit_transform =
    {
        1.0f,0.0f,0.0f,0.0f,
        0.0f,1.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,0.0f,
        0.0f,0.0f,0.0f,1.0f,
    };

    Hit_Result local_result;
    local_result.distance = FLT_MAX;
    if (get_model()->raycast(localStart, localEnd, unit_transform, &local_result))
    {

        //　前回のローカル空間から　”現在の”ワールド空間へ変換
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&get_transform()->get_matrix());
        DirectX::XMVECTOR localHitPosition = DirectX::XMLoadFloat3(&local_result.position);
        DirectX::XMVECTOR localHitNormal = DirectX::XMLoadFloat3(&local_result.normal);

        DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(localHitPosition, WorldTransform);
        DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(localHitNormal, WorldTransform);
        DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
        DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
        float distance;
        DirectX::XMStoreFloat(&distance, WorldCrossLength);


        // 変換結果
        DirectX::XMStoreFloat3(&hit_result_->position, WorldPosition);
        DirectX::XMStoreFloat3(&hit_result_->normal, DirectX::XMVector3Normalize(WorldNormal));
        hit_result_->distance = distance;
        hit_result_->material_index = local_result.material_index;

        //const DirectX::XMFLOAT3 euler = get_transform()->get_euler(); // 欠陥 : 角度によって一意に値が求まらない場合があった

        // ↑欠陥があったので 個別に作った変数m_eulerで対応した
        hit_result_->rotation.x = m_euler.x - old_euler.x;
        hit_result_->rotation.y = m_euler.y - old_euler.y;
        hit_result_->rotation.z = m_euler.z - old_euler.z;

        return true;
    }

    return false;
}
