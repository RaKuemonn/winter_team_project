
#include "clear_judge.h"

Clear_Judge::Clear_Judge(Stage_Select stage_enum_, const DirectX::XMFLOAT3& player_position_, short* boss_hp_)
{

    if(stage_enum_ == Stage_Select::STAGE_BOSS)
    {
        ptr_boss_hp = boss_hp_;

        judge_function = [&]() { return Clear_Judge::boss_dead(); };
    }

    else
    {
        m_goal_judge = std::make_unique<Goal_Judge>(stage_enum_, player_position_);

        judge_function = [&]() { return Clear_Judge::area_judge(); };
    }
    
}

bool Clear_Judge::judge()
{
    bool i = judge_function();
    return i;
}


bool Clear_Judge::area_judge()
{
    return m_goal_judge->judge();
}

bool Clear_Judge::boss_dead()
{
    constexpr short zero = 0;
    bool i = (*ptr_boss_hp <= zero);
    return i;
}
