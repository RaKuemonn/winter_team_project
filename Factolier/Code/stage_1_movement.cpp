
#include "stage_1_movement.h"
#include "scene_manager.h"
#include "model.h"
#include "collision.h"
#include "transform.h"


stage_1_movement::stage_1_movement(Scene_Manager* ptr_scene_manager_)
{
    load_model(ptr_scene_manager_->model_manager()->load_model("./Data/nico.fbx"));
}

void stage_1_movement::update(const float elapsed_time)
{
    old_transform = get_transform()->get_matrix();

    get_transform()->Update();
}

bool stage_1_movement::ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)
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
    if (get_model()->raycast(localStart,localEnd,unit_transform,&local_result))
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

        const DirectX::XMFLOAT3 euler = get_transform()->get_euler();

        hit_result_->rotation.x = euler.x - old_euler.x;
        hit_result_->rotation.y = euler.y - old_euler.y;
        hit_result_->rotation.z = euler.z - old_euler.z;

        return true;
    }


    return false;
}
