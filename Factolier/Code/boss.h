#pragma once
#include "base_move_phase.h"
#include "enemy.h"
#include "Timer.h"


// ‚±‚ÌƒNƒ‰ƒX‚ğ¶¬‚·‚é‚Æ‘Ì•”•ª‚à¶¬‚³‚ê‚é

class Boss : public Enemy
{
public:
    Boss(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& target_position_);
    ~Boss()override = default;

    void init() override{};
    void update(const float elapsed_time_) override;

private:
    void init_move();
    void init_move_1();
    void init_move_2();
    void init_move_3();
    void init_move_4();

private:
    static constexpr int move_phase_size    = 4;
    static constexpr int body_size          = 3;
    Timer m_timer;

    std::weak_ptr<class Boss_Body> wkp_bodies[body_size]    = {};
    std::unique_ptr<Move_Phases<Boss>> m_move_phases        = nullptr;
    Move_Phase<Boss>* m_move_phase[move_phase_size]         = {};
};

