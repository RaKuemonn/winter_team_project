#pragma once
#include <DirectXMath.h>
#include <functional>
#include <memory>

#include "Goal_Judge.h"
#include "option_manager.h"

class Clear_Judge final
{
public:
    Clear_Judge(Stage_Select stage_enum_,const DirectX::XMFLOAT3& player_position_ , short* boss_hp_);
    ~Clear_Judge() = default;


    bool judge();

private:
    bool area_judge();
    bool boss_dead();
    std::function<bool()> judge_function;


    std::unique_ptr<Goal_Judge> m_goal_judge = nullptr;
    short* ptr_boss_hp = nullptr;
};
