#include "boss.h"

#include "ability.h"
#include "model_filepaths.h"
#include "scene_manager.h"
#include "entity_manager.h"
#include "transform.h"

#include "boss_body_1.h"
#include "boss_body_2.h"
#include "boss_body_3.h"
using namespace body;

constexpr short boss_hp = 3;

// �ʖ��G�C���A�X
using _this_type_ = Boss;

#include "idle.h"

Boss::Boss(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_head, target_position_, false)
{
    m_timer.Initialize(nullptr, COUNT::UP, 0.0f);

    // �s���J��
    init_move();

    // TODO: �{�X�̗̑͗v����
    // �̗͂̐ݒ�
    get_ability().set_init(4);

    

    
    {
        std::shared_ptr<Boss_Body> body_1 = std::make_shared<Boss_Body_1>(ptr_scene_manager_, target_position_);
        std::shared_ptr<Boss_Body> body_2 = std::make_shared<Boss_Body_2>(ptr_scene_manager_, target_position_, body_1);
        std::shared_ptr<Boss_Body> body_3 = std::make_shared<Boss_Body_3>(ptr_scene_manager_, target_position_, body_1, body_2);

        wkp_bodies[0] = body_1;
        wkp_bodies[1] = body_2;
        wkp_bodies[2] = body_3;


        std::shared_ptr<Entity> e_body_1 = body_1;
        std::shared_ptr<Entity> e_body_2 = body_2;
        std::shared_ptr<Entity> e_body_3 = body_3;
        Entity_Manager::instance().spawn_register(e_body_1);
        Entity_Manager::instance().spawn_register(e_body_2);
        Entity_Manager::instance().spawn_register(e_body_3);
    }
    
    get_transform()->set_scale({ scale,scale,scale });
    const DirectX::XMFLOAT3& under_body_position        = wkp_bodies[2].lock()->get_position();
    set_position({ under_body_position.x,under_body_position.y + body_height ,under_body_position.z });
    get_transform()->Update();

}

void Boss::update(const float elapsed_time_)
{
    // �s���J�ڗp�̎��ԍX�V
    m_timer.Update(elapsed_time_);

    // �̗͂̊m�F
    check_wkp_body();
    
    // �e�̍s���J��
    m_move_phases->update(elapsed_time_, *this);

    look_at_target(elapsed_time_);

    // �p���̍X�V
    get_transform()->Update();

    //// ���f���̍X�V
    //get_model()->play_animation(elapsed_time_, 0);
}


void Boss::init_move()
{

    // �q�̍s���J�ڂ����̐���
    for (int i = 0; i < move_phase_size; ++i)
    {
        m_move_phase[i] = new Move_Phase<_this_type_>();
    }

    init_move_1();
    init_move_2();
    init_move_3();
    init_move_4();


    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////


    // �e�̍s���J�ڂ̐���
    m_move_phases = std::make_unique<Move_Phases<_this_type_>>();

    // �e�̏����s��
    m_move_phases->set(m_move_phase[0])
        .add_transition(m_move_phase[0], m_move_phase[1], [&](float) {return (get_ability().get_hp() < boss_hp); })
        .add_transition(m_move_phase[1], m_move_phase[2], [&](float) {return (get_ability().get_hp() < boss_hp - 1); })
        .add_transition(m_move_phase[2], m_move_phase[3], [&](float) {return (get_ability().get_hp() < boss_hp - 2); });
    
}

void Boss::init_move_1()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // �q1�̍s���J��
    m_move_phase[0]->set(idle);
}

void Boss::init_move_2()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // �q2�̍s���J��
    m_move_phase[1]->set(idle);
}

void Boss::init_move_3()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // �q3�̍s���J��
    m_move_phase[2]->set(idle);
}

void Boss::init_move_4()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // �q4�̍s���J��
    m_move_phase[3]->set(idle);
}


void Boss::look_at_target(const float elapsed_time_)
{
    const DirectX::XMFLOAT3 axis_x = get_axis_x();
    const DirectX::XMVECTOR right_direction = DirectX::XMLoadFloat3(&axis_x);

    const DirectX::XMFLOAT3 axis_z = get_axis_z();
    const DirectX::XMVECTOR front_direction = DirectX::XMLoadFloat3(&axis_z);

    const DirectX::XMFLOAT3 target_position = { get_target_position().x,0.0f,get_target_position().z };
    const DirectX::XMFLOAT3 position = { get_position().x,0.0f,get_position().z };

    const DirectX::XMVECTOR to_target_direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&target_position), DirectX::XMLoadFloat3(&position)));


    const float dot_right_or_left = DirectX::XMVectorGetX(DirectX::XMVector3Dot(to_target_direction, right_direction));
    // sign = ����     +1.0f �� - 1.0f �̂ǂ��炩������
    const float sign = (dot_right_or_left > 0.0f) ? +1.0f : -1.0f;
    const float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(to_target_direction, front_direction));           // ���K�����ꂽ�x�N�g���ɂ�����

    if (std::abs(dot) >= 1.0f - FLT_EPSILON) return;


    const DirectX::XMVECTOR cross = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(front_direction, to_target_direction));   // ���K�����ꂽ�x�N�g���ɂ��O��
    const float radian = elapsed_time_ == 0.0f ?
        acosf(dot)/*��]��*/ /*��]����*/ :
        acosf(dot)/*��]��*/ /*��]����*/ * 2.0f * elapsed_time_/*�t���[�����[�g*/;

    // TODO : ���͉�]���x����� , ��Ԃ��Ď��R�ɂ���

    const DirectX::XMVECTOR xmvec_quaternion = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationAxis(cross, radian));
    DirectX::XMFLOAT4 xmf4_quaternion;
    DirectX::XMStoreFloat4(&xmf4_quaternion, xmvec_quaternion);

    // ��]������
    add_quaternion(xmf4_quaternion);
}

void Boss::check_wkp_body()
{
    short hp = boss_hp;
    for(int i = 0; i < body_size; ++i)
    {
        if(wkp_bodies[i].expired() == false) continue;

        // �Q�Ƃ��Ȃ���
        --hp;
    }

    get_ability().set_init(hp);

}

