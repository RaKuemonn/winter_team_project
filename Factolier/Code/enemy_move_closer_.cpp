
#include "enemy_move_closer_.h"
#include "transform.h"
#include "model.h"
#include "scene_manager.h"
#include "model_filepaths.h"
#include "utility.h"
#include "area_mover.h"
#include "target_mover.h"



inline void look_target(Enemy_Move_Closer_& me, const DirectX::XMFLOAT3& target_)
{
    const DirectX::XMFLOAT3 axis_x = me.get_axis_x();
    const DirectX::XMVECTOR right_direction = DirectX::XMLoadFloat3(&axis_x);

    const DirectX::XMFLOAT3 axis_z = me.get_axis_z();
    const DirectX::XMVECTOR front_direction = DirectX::XMLoadFloat3(&axis_z);

    const DirectX::XMFLOAT3 target_position = { target_.x,0.0f,target_.z };
    const DirectX::XMFLOAT3 position = { me.get_position().x,0.0f,me.get_position().z };

    const DirectX::XMVECTOR to_target_direction = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&target_position), DirectX::XMLoadFloat3(&position)));


    const float dot_right_or_left = DirectX::XMVectorGetX(DirectX::XMVector3Dot(to_target_direction, right_direction));
    // sign = 符号     +1.0f か - 1.0f のどちらかが入る
    const float sign = (dot_right_or_left > 0.0f) ? +1.0f : -1.0f;
    const float dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(to_target_direction, front_direction));           // 正規化されたベクトルによる内積

    if (std::abs(dot) >= 1.0f - FLT_EPSILON) return;


    const DirectX::XMVECTOR cross = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(front_direction, to_target_direction));   // 正規化されたベクトルによる外積
    const float radian = acosf(dot)/*回転量*/;


    const DirectX::XMVECTOR xmvec_quaternion = DirectX::XMQuaternionNormalize(DirectX::XMQuaternionRotationAxis(cross, radian));
    DirectX::XMFLOAT4 xmf4_quaternion;
    DirectX::XMStoreFloat4(&xmf4_quaternion, xmvec_quaternion);

    // 回転させる
    me.add_quaternion(xmf4_quaternion);
}



Enemy_Move_Closer_::Enemy_Move_Closer_(Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_) : Enemy(ptr_scene_manager_, (filename_) ? filename_ : (Model_Paths::Entity::enemy_move_closer), target_position_, true)
{
    m_timer.Initialize(nullptr, COUNT::UP, 0.0f);

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
    // 死んでいるかどうか
    if (check_im_die()) return;

    // テリトリーの初期設定 (初期位置から固定サイズの大きさで設定される)
    init_define_area_parameters();

    // 行動遷移用の時間更新
    m_timer.Update(elapsed_time_);

    // 行動遷移
    m_move_phase->update(elapsed_time_, *this);

    // 速度更新
    update_velocity(elapsed_time_);   // 重力がかかっている

    // 位置の更新
    add_position(m_velocity->get() * elapsed_time_);

    // エリア内か判断
    judge_in_area();

    // 姿勢の更新
    get_transform()->Update();

    // 死んだかどうか

    // モデルの更新
    get_model()->play_animation(elapsed_time_, 0);
}


void Enemy_Move_Closer_::init_define_area_parameters()
{
    if (m_area_origin_position.x != undefine_area.x &&
        m_area_origin_position.y != undefine_area.y &&
        m_area_origin_position.z != undefine_area.z) return;

    // 全て undefine_area と同じなら

    m_area_origin_position = get_latest_position();

    // TODO: 敵の行動範囲が固定　-> 設定できるように
    m_area_size = { 14.0f,0.0f,26.0f };

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


void Enemy_Move_Closer_::judge_in_area()
{
    out_area();
}

#undef min
#undef max

bool Enemy_Move_Closer_::out_area()
{
    DirectX::XMFLOAT3 position = get_position();

    const DirectX::XMFLOAT3 min = m_area_origin_position - m_area_size * 0.5f;
    const DirectX::XMFLOAT3 max = m_area_origin_position + m_area_size * 0.5f;


    bool hit = false;

    // x_z 平面上でエリア外か判別する
    if (position.x < min.x) { hit = true; position.x = min.x; }
    if (position.x > max.x) { hit = true; position.x = max.x; }
    //if (position.y > min.y) return false;
    //if (position.y < max.y) return false;
    if (position.z < min.z) { hit = true; position.z = min.z; }
    if (position.z > max.z) { hit = true; position.z = max.z; }

    if(hit)
    {
        set_position(position);
    }

    // エリア外
    return hit;
}