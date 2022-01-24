#pragma once

#include "enemy.h"
#include "transform.h"
#include "timer.h"
#include "base_move_phase.h"


class Enemy_Move_Closer_ : public Enemy
{
public:
    Enemy_Move_Closer_(class Scene_Manager* ptr_scene_manager_, const char* filename_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Move_Closer_()override = default;

    virtual void init() override = 0;
    void update(const float elapsed_time_) override;

    void set_area_size(const DirectX::XMFLOAT3 size_)
    {
        m_area_size = size_;
    }

private:
    void init_define_area_parameters();

    static bool is_find_target(const Enemy_Move_Closer_& me);    // エリア内で敵を見つけたかどうか
    
    void init_in_area();                                        // 初期化
    void find_in_area();                                        // エリア内の探索位置を決定
    void look_in_area();                                        // 探索位置に向く

    bool stop_velocity()
    {
        m_velocity->set({ 0.0f,0.0f,0.0f });
        return true;
    }

    void judge_in_area();
    bool out_area();


private:
    static constexpr DirectX::XMFLOAT3 undefine_area = { -99999.0f,-99999.0f,-99999.0f };

    Timer m_timer;
    DirectX::XMFLOAT3 m_area_target_position    = {};
    DirectX::XMFLOAT3 m_area_origin_position    = undefine_area;        // 行動範囲の中心      (行動範囲はbox)
    DirectX::XMFLOAT3 m_area_size               = {};                   // 行動範囲の大きさ    (大きさはboxの頂点から頂点までの辺の長さ)

    std::unique_ptr<Move_Phase<Enemy_Move_Closer_>> m_move_phase = nullptr;

};
