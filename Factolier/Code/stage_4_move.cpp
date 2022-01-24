
#include "stage_4_move.h"
#include "scene_manager.h"
#include "transform.h"
#include "model_filepaths.h"
#include "collision.h"

Stage_4_Move::Stage_4_Move(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position_) : Stage(position_)
{
    load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Stage::stage_4_propeller, true));

    get_transform()->set_scale(stage::scale);
    get_transform()->set_euler({ 0.0f, -3.14f, 0.0f });
    get_transform()->Update();
}

void Stage_4_Move::update(const float elapsed_time)
{
    old_transform = get_transform()->get_matrix();
    old_euler = m_euler;

    DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

    DirectX::XMVECTOR Pos = DirectX::XMLoadFloat3(&get_transform()->get_position());

    float start_to_pos_length = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(Pos, Start)));
    float start_to_pos_dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMVectorSubtract(Pos, Start), Vec));

    const float length = DirectX::XMVectorGetX(DirectX::XMVector3Length(Vec));

    if (move_ratio < 0.0f && start_to_pos_dot < 0.0f)
    {
        move_ratio = 1.0f;
        Pos = Start;
    }

    if (start_to_pos_length > length)
    {
        move_ratio = -1.0f;
        Pos = End;
    }

    constexpr float speed = 7.0f;
    float now_speed = move_ratio * speed * elapsed_time;
    DirectX::XMFLOAT3 position;
    DirectX::XMStoreFloat3(&position, DirectX::XMVectorMultiplyAdd(DirectX::XMVector3Normalize(Vec), DirectX::XMVectorReplicate(now_speed), Pos));
    get_transform()->set_position(position);
    /*const float speed = move_speed * elapsed_time;
    float speedRate = speed / length;
    move_rate_ += speedRate;

    if (move_rate_ <= 0.0f || move_rate_ >= 1.0f)
    {
        move_speed = -move_speed;
    }*/

    /*DirectX::XMFLOAT3 position;
    DirectX::XMStoreFloat3(&position, DirectX::XMVectorLerp(Start, End, move_rate_));
    get_transform()->set_position(position)*/;

    get_transform()->Update();
}

bool Stage_4_Move::ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)
{

    // �O��̃��[���h�s��ŋt�s����Z�o
    DirectX::XMMATRIX WorldOldTransform = DirectX::XMLoadFloat4x4(&old_transform);
    DirectX::XMMATRIX InverseWorldOldTransform = DirectX::XMMatrixInverse(nullptr, WorldOldTransform);

    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);

    // ���[���h��Ԃ̎n�_�ƏI�_�����[�J����ԏ�ɕϊ�
    DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldOldTransform);
    DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldOldTransform);
    // ���[�J����Ԃ̃��C�̎n�_�ƏI�_
    DirectX::XMFLOAT3 localStart, localEnd;
    DirectX::XMStoreFloat3(&localStart, Start);
    DirectX::XMStoreFloat3(&localEnd, End);

    // �P�ʍs��
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

        //�@�O��̃��[�J����Ԃ���@�h���݂́h���[���h��Ԃ֕ϊ�
        DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&get_transform()->get_matrix());
        DirectX::XMVECTOR localHitPosition = DirectX::XMLoadFloat3(&local_result.position);
        DirectX::XMVECTOR localHitNormal = DirectX::XMLoadFloat3(&local_result.normal);

        DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(localHitPosition, WorldTransform);
        DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(localHitNormal, WorldTransform);
        DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
        DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
        float distance;
        DirectX::XMStoreFloat(&distance, WorldCrossLength);


        // �ϊ�����
        DirectX::XMStoreFloat3(&hit_result_->position, WorldPosition);
        DirectX::XMStoreFloat3(&hit_result_->normal, DirectX::XMVector3Normalize(WorldNormal));
        hit_result_->distance = distance;
        hit_result_->material_index = local_result.material_index;

        //const DirectX::XMFLOAT3 euler = get_transform()->get_euler(); // ���� : �p�x�ɂ���Ĉ�ӂɒl�����܂�Ȃ��ꍇ��������

        // �����ׂ��������̂� �ʂɍ�����ϐ�m_euler�őΉ�����
        hit_result_->rotation.x = m_euler.x - old_euler.x;
        hit_result_->rotation.y = m_euler.y - old_euler.y;
        hit_result_->rotation.z = m_euler.z - old_euler.z;

        return true;
    }

    return false;
}