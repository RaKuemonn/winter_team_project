
#include "enemy_move_closer.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "model_filepaths.h"
#include "utility.h"


inline void move_front(const DirectX::XMFLOAT3 axis_z, Velocity* velocity)
{
    constexpr float speed = 1.5f;
    velocity->add(axis_z * speed);
}


Enemy_Move_Closer::Enemy_Move_Closer(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, Model_Paths::Entity::enemy_move_closer, target_position_)
{
    m_timer.Initialize(nullptr, COUNT::UP, 0.0f);

    m_area_origin_position = {};
    m_area_size = { 5.0f,0.0f,10.0f };

    constexpr float scale = 0.01f;
    get_transform()->set_scale({ scale,scale,scale });
    get_transform()->Update();
}

void Enemy_Move_Closer::update(const float elapsed_time_)
{
    // 行動遷移
    move_phase(elapsed_time_);

    m_velocity->update(elapsed_time_);

    add_position(m_velocity->get() * elapsed_time_);

    // 姿勢の更新
    get_transform()->Update();

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);
}


bool Enemy_Move_Closer::is_find_target(const Enemy_Move_Closer& me)
{
    // 自身の周囲10.0f以内の距離にいれば 見つけた判定
    const float length = DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&me.get_position()), DirectX::XMLoadFloat3(&me.get_target_position()))));

    return (length <= 10.0f);
}


void Enemy_Move_Closer::move_phase(const float elapsed_time_)
{
    // 目標が近くにいるか判別
    if (m_is_find_target == false)                  // 見つけていない
    {
        m_is_find_target = is_find_target(*this);   // このフレームで見つけたか

        if (m_is_find_target == false)              // このフレームで見つけていない (早期return)
        {
            // 近くにいない場合 エリア内の探索
            search_in_area(elapsed_time_);
            return;
        }

        else                                        // このフレームで見つけた       (そのまま下の処理へ)
        {
            m_is_move_phase_init = true;
        }
    }

    // 近くにいた場合 目標に向かって動き出す
    seek_to_target(elapsed_time_);

}




void Enemy_Move_Closer::search_in_area(const float elapsed_time_)
{
    if (m_is_move_phase_init)
    {
        init_in_area(elapsed_time_);
    }

    m_timer.Update(elapsed_time_);

    const float timer = m_timer.NowTime();

    if (timer < 3.0f)
    {
        move_in_area(elapsed_time_);
    }
    else
    {
        if (timer < 6.0f)
        {
            idle_in_area(elapsed_time_);
        }
        else
        {
            deinit_in_area(elapsed_time_);
        }
    }
}

void Enemy_Move_Closer::init_in_area(const float elased_time_)
{
    find_in_area(elased_time_);
    look_in_area(elased_time_);
    m_is_move_phase_init = false;
    m_timer.Reset();
}

void Enemy_Move_Closer::deinit_in_area(const float elased_time_)
{
    // リセット
    m_is_move_phase_init = true;
}

void Enemy_Move_Closer::find_in_area(const float elapsed_time_)
{
    m_area_target_position = random_range_box(m_area_origin_position, m_area_size);
}

void Enemy_Move_Closer::look_in_area(const float elapsed_time_)
{
    look_target(*this, m_area_target_position, 0.0f);
}

void Enemy_Move_Closer::move_in_area(const float elapsed_time_)
{
    // 前フレームの前ベクトル
    move_front(get_axis_z(), m_velocity.get());
}

void Enemy_Move_Closer::idle_in_area(const float elapsed_time_)
{
    // 何もしない
}




void Enemy_Move_Closer::seek_to_target(const float elapsed_time_)
{
    if (is_find_target(*this))
    {
        m_timer.Reset();
        m_timer.Add(0.6f);
    }

    if (m_is_move_phase_init)
    {
        init_to_target(elapsed_time_);
    }

    look_to_target(elapsed_time_);


    m_timer.Update(elapsed_time_);
    const float timer = m_timer.NowTime();


    if (timer < 0.6f)return; // 見つけてから 0.6秒 経ったら移動し始める


    if (timer < 3.0f) 
    {
        move_to_target(elapsed_time_);
    }
         
    else // 3秒経ったら一度リセット
    {
        deinit_to_target(elapsed_time_);
    }
}

void Enemy_Move_Closer::init_to_target(const float elapsed_time_)
{
    m_is_move_phase_init = false;
    m_timer.Reset();
    m_velocity->set({ 0.0f,0.0f,0.0f });
}

void Enemy_Move_Closer::deinit_to_target(const float elased_time_)
{
    m_is_move_phase_init = true;
    m_is_find_target = false;
}

void Enemy_Move_Closer::look_to_target(const float elapsed_time_)
{
    look_target(*this,get_target_position() ,elapsed_time_);
}

void Enemy_Move_Closer::move_to_target(const float elapsed_time_)
{
    // 前フレームの前ベクトル
    move_front(get_axis_z(), m_velocity.get());
}
