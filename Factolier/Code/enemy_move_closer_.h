#pragma once

#include "enemy.h"
#include "transform.h"
#include "timer.h"
#include "base_move_phase.h"


class Enemy_Move_Closer_ final : public Enemy
{
public:
    Enemy_Move_Closer_(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Enemy_Move_Closer_()override = default;

    void init() override {};
    void update(const float elapsed_time_) override;

private:
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
    Timer m_timer;
    DirectX::XMFLOAT3 m_area_target_position    = {};
    DirectX::XMFLOAT3 m_area_origin_position    = {};
    DirectX::XMFLOAT3 m_area_size               = {};

    std::unique_ptr<Move_Phase<Enemy_Move_Closer_>> m_move_phase = nullptr;

};
