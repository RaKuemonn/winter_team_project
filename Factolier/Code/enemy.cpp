
#include "enemy.h"
#include "transform.h"
#include "scene_manager.h"
#include "collide_detection_entity.h"
#include "entity_manager.h"


Enemy::Enemy(Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_, bool default_collde_is_, bool boss_) : target_position(target_position_)
{

    set_ptr_scene_manager(ptr_scene_manager_);
    load_model(get_scene_manager()->model_manager()->load_model(filename_, true));
    set_tag(Tag::Enemy);

    m_velocity->set_mass(1.0f);
    m_velocity->set_friction(0.7f);

    // �U�R�G�p�̓����蔻��
    if(default_collde_is_)
    {
        constexpr float radius = 0.8f;
        std::shared_ptr<Collide_Detection> collide_detection;
        if (boss_)
        {
            collide_detection = std::make_shared<Collide_Detection>(ptr_scene_manager_, radius * 5.0f);
        }
        else
        {
            collide_detection = std::make_shared<Collide_Detection>(ptr_scene_manager_, radius);
        }
        wkp_collide_detection                                   = collide_detection;

        std::shared_ptr<Entity> e_collide_detection             = collide_detection;
        Entity_Manager::instance().spawn_register(e_collide_detection);
    }

}

void Enemy::update_velocity(const float elapsed_time_)
{
    constexpr DirectX::XMFLOAT3 gravity = { 0.0f,-3.0f * 9.8f,0.0f };
    m_velocity->add(gravity);

    // ���x�̍X�V
    m_velocity->update(elapsed_time_);
}

bool Enemy::check_im_die()
{
    
    // expired��false�̂Ƃ��Q�Ƃ���Ă���
    if (wkp_collide_detection.expired() == false) // �Q�Ƃ��؂�鏈��collide_detection���������̂́Acollision_manager�ō폜��������������@
    {
        wkp_collide_detection.lock()->set_position(get_position());
        return false;
    }
    //TODO: �Ă��� ->�@�E�̏����ł�낤�Ƃ���ƁA�폜�����O�ł�����ʂ�̂ł��̃U�R�G�������Ȃ�..... entity_manager�̍폜�������h�h�O�Ɉ�ǉ��h�h����

    


    // �ȉ��͎Q�Ƃ��؂�Ă���Ƃ��̏���


    // entity��update���remove_entity�̏������ʂ�̂ŁAentity�t���[���Ԃɍ폜�o�^�����
    // ����entity�͏�����̂�,���̂܂܍폜�o�^����


    Entity_Manager::instance().remove_register(this);

    return true;
}
