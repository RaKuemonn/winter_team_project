#pragma once

#include <DirectXMath.h>

#include "option_manager.h"

class Goal_Judge final
{
public:
    Goal_Judge(Stage_Select stage_enum_, const DirectX::XMFLOAT3& player_position_);
    ~Goal_Judge() = default;


    bool judge();

private:
    void init_stage_1();
    void init_stage_2();
    void init_stage_3();
    void init_stage_4();

    bool area_judge();

    const DirectX::XMFLOAT3& player_position;
    DirectX::XMFLOAT3 m_area_origin_position = {};
    DirectX::XMFLOAT3 m_area_size = {};
};
