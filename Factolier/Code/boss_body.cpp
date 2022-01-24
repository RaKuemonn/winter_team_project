#include "boss_body.h"

#include "ability.h"
#include "transform.h"
#include "collide_detection_entity.h"
#include "entity_manager.h"
#include "utility.h"
using namespace body;

inline void collide_detection_set_position(std::weak_ptr<Collide_Detection> collide_detection, const DirectX::XMFLOAT3& axis_z_ , const DirectX::XMFLOAT3& body_position, const float rotated_y, const int i)
{
    constexpr float body_radius = 20.0f;
    constexpr float radian_60 = DirectX::XMConvertToRadians(60.0f);
    const DirectX::XMVECTOR y_axis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

    const DirectX::XMVECTOR axis_z = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&axis_z_), body_radius);
    DirectX::XMVECTOR vec = DirectX::XMVector3Rotate(axis_z, DirectX::XMQuaternionRotationAxis(y_axis, rotated_y + radian_60 * i));

    DirectX::XMFLOAT3 collision_detection_position = {};

    DirectX::XMStoreFloat3(&collision_detection_position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&body_position), vec));
    constexpr float height = 0.0f;
    collision_detection_position.y += height;
    collide_detection.lock()->set_position(collision_detection_position);
}


Boss_Body::Boss_Body(Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_, const short hp_) : Enemy(ptr_scene_manager_, filename_, target_position_, false)
{
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();

    // �̗̂̑͂�ݒ�
    get_ability().set_init(hp_);


    for(int i = 0; i < collide_array_size; ++i)
    {
        // TODO : ���̑傫�����Ⴄ���A�S�ē����傫���̓����蔻��ɂȂ��Ă���
        static constexpr float collide_radius = 10.0f;
        std::shared_ptr<Collide_Detection> collide_detection = std::make_shared<Collide_Detection>(ptr_scene_manager_,collide_radius);

        //�@�ʒu�ݒ�
        collide_detection_set_position(collide_detection, get_axis_z(), get_position(), m_euler_y, i);
        collide_detection->update(0.0f);

        wkp_collide_detection[i] = collide_detection;


        std::shared_ptr<Entity> e_collide_detection = collide_detection;
        Entity_Manager::instance().spawn_register(e_collide_detection);
    }

}

void Boss_Body::update_collision_detection()
{
    for(int i = 0; i < collide_array_size; ++i)
    {
        if(wkp_collide_detection[i].expired()) continue;    // true�̂Ƃ��Q�Ɛ悪�Ȃ�

        // ��]����,�ʒu�X�V
        collide_detection_set_position(wkp_collide_detection[i], get_axis_z(), get_position(), m_euler_y, i);

        // TODO: ��΂� �����ōX�V�����Ă��� (�������̊֌W�ł����ł��Ȃ��ƈʒu��1�t���[���x���)
        wkp_collide_detection[i].lock()->update(0.0f);
    }

}

bool Boss_Body::check_im_hp()
{
    // ���łɎ���ł�����`�F�b�N���Ȃ�
    if (get_was_died())return true;



    // �Ԃꂽ���𐔂���
    int not_reference_size = 0;
    for(int i = 0; i < collide_array_size; ++i)
    {
        if(wkp_collide_detection[i].expired() == false) continue;

        ++not_reference_size;
    }

    // �Ԃꂽ�����̗͂�葽�����
    if(not_reference_size < get_ability().get_hp()) return false;

    // ����ł���
    set_was_died();
    return true;
}
