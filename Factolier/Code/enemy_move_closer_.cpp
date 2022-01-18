
#include "enemy_move_closer_.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "model_filepaths.h"
#include "utility.h"
#include "area_mover.h"
#include "target_mover.h"



Enemy_Move_Closer_::Enemy_Move_Closer_(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, Model_Paths::Entity::enemy_move_closer, target_position_)
{
    m_timer.Initialize(nullptr, COUNT::UP, 0.0f);

    m_area_origin_position = {};
    m_area_size = { 5.0f,0.0f,10.0f };


    // 別名エイリアス
    using _this_type_ = Enemy_Move_Closer_;

    Base_Move<_this_type_>* area_mover      = new Area_Mover<_this_type_>();
    Base_Move<_this_type_>* target_mover    = new Target_Mover<_this_type_>();
    m_move_phase                            = std::make_unique<Move_Phase<_this_type_>>();

    // 初期行動
    m_move_phase->set(area_mover)
    // 遷移元と遷移先と遷移条件
        .add_transition(area_mover, area_mover,     [&](float) {return ((is_find_target(*this) == false) && (m_timer.NowTime() > 3.0f))     ? stop_velocity()/*true返す*/ : false; })
        .add_transition(area_mover, target_mover,   [&](float) {return is_find_target(*this)                                                ? stop_velocity()/*true返す*/ : false; })
        .add_transition(target_mover, area_mover,   [&](float) {return (is_find_target(*this) == false) && (m_timer.NowTime() > 3.0f); })
        .add_transition(target_mover, target_mover, [&](float) {return (is_find_target(*this) && (m_timer.NowTime() > 0.6f)); })
    // 初期化、終了
        .set_initialize(area_mover,     [&]() {this->init_in_area();        m_timer.Reset();})
        .set_finalize(area_mover,           [&]() { m_timer.Reset();})
        .set_initialize(target_mover,   [&]() { m_timer.Reset();})
        .set_finalize(target_mover,     [&]() { m_timer.Reset();});


    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Enemy_Move_Closer_::update(const float elapsed_time_)
{
    // 行動遷移用の時間更新
    m_timer.Update(elapsed_time_);

    // 行動遷移
    m_move_phase->update(elapsed_time_, *this);

    // 速度更新
    update_velocity(elapsed_time_);   // 重力がかかっている

    // 位置の更新
    add_position(m_velocity->get() * elapsed_time_);

    // 姿勢の更新
    get_transform()->Update();

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);
}


bool Enemy_Move_Closer_::is_find_target(const Enemy_Move_Closer_& me)
{
    // 自身の周囲10.0f以内の距離にいれば 見つけた判定
    const float length = DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&me.get_position()), DirectX::XMLoadFloat3(&me.get_target_position()))));

    return (length <= 10.0f);
}

void Enemy_Move_Closer_::init_in_area()
{
    find_in_area();
    look_in_area();
}

void Enemy_Move_Closer_::find_in_area()
{
    m_area_target_position = random_range_box(m_area_origin_position, m_area_size);
}

void Enemy_Move_Closer_::look_in_area()
{
    look_target(*this, m_area_target_position);
}



