#include "boss.h"

#include "ability.h"
#include "model_filepaths.h"
#include "scene_manager.h"
#include "transform.h"

// �ʖ��G�C���A�X
using _this_type_ = Boss;

#include "idle.h"

Boss::Boss(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_head, target_position_)
{
    m_timer.Initialize(nullptr, COUNT::UP, 0.0f);

    // �s���J��
    init_move();

    // TODO: �{�X�̗̑͗v����
    // �̗͂̐ݒ�
    get_ability().set_init(4);

    constexpr float scale = 0.5f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Boss::update(const float elapsed_time_)
{
    // �s���J�ڗp�̎��ԍX�V
    m_timer.Update(elapsed_time_);

    // �e�̍s���J��
    m_move_phases->update(elapsed_time_, *this);

    // ���x�X�V
    update_velocity(elapsed_time_);   // �d�͂��������Ă���

    // �ʒu�̍X�V
    add_position(m_velocity->get() * elapsed_time_);

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
        .add_transition(m_move_phase[0], m_move_phase[1], [&](float) {return (get_ability().get_hp() <= 3); })
        .add_transition(m_move_phase[1], m_move_phase[2], [&](float) {return (get_ability().get_hp() <= 2); })
        .add_transition(m_move_phase[2], m_move_phase[3], [&](float) {return (get_ability().get_hp() <= 1); });
    
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
