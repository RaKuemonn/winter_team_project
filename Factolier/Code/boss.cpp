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

// 別名エイリアス
using _this_type_ = Boss;

#include "idle.h"

Boss::Boss(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, Model_Paths::Entity::enemy_boss_head, target_position_)
{
    m_timer.Initialize(nullptr, COUNT::UP, 0.0f);

    // 行動遷移
    init_move();

    // TODO: ボスの体力要調節
    // 体力の設定
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
    // 行動遷移用の時間更新
    m_timer.Update(elapsed_time_);
    
    // 親の行動遷移
    m_move_phases->update(elapsed_time_, *this);

    // 姿勢の更新
    get_transform()->Update();

    //// モデルの更新
    //get_model()->play_animation(elapsed_time_, 0);
}


void Boss::init_move()
{

    // 子の行動遷移たちの生成
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


    // 親の行動遷移の生成
    m_move_phases = std::make_unique<Move_Phases<_this_type_>>();

    // 親の初期行動
    m_move_phases->set(m_move_phase[0])
        .add_transition(m_move_phase[0], m_move_phase[1], [&](float) {return (get_ability().get_hp() <= 3); })
        .add_transition(m_move_phase[1], m_move_phase[2], [&](float) {return (get_ability().get_hp() <= 2); })
        .add_transition(m_move_phase[2], m_move_phase[3], [&](float) {return (get_ability().get_hp() <= 1); });
    
}

void Boss::init_move_1()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // 子1の行動遷移
    m_move_phase[0]->set(idle);
}

void Boss::init_move_2()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // 子2の行動遷移
    m_move_phase[1]->set(idle);
}

void Boss::init_move_3()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // 子3の行動遷移
    m_move_phase[2]->set(idle);
}

void Boss::init_move_4()
{
    Base_Move<_this_type_>* idle = new Idle<_this_type_>();

    // 子4の行動遷移
    m_move_phase[3]->set(idle);
}
