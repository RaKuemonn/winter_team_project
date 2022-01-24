
#include "goal_judge.h"
#include "utility.h"

Goal_Judge::Goal_Judge(Stage_Select stage_enum_, const DirectX::XMFLOAT3& player_position_) : player_position(player_position_)
{



    switch (stage_enum_)
    {

    case Stage_Select::STAGE_1:
    {
        init_stage_1();
        break;
    }

    case Stage_Select::STAGE_2:
    {
        init_stage_2();
        break;
    }

    case Stage_Select::STAGE_3:
    {
        init_stage_3();
        break;
    }

    case Stage_Select::STAGE_4:
    {
        init_stage_4();
        break;
    }

    }
}

bool Goal_Judge::judge()
{
    return area_judge();
}



bool Goal_Judge::area_judge()
{
    const DirectX::XMFLOAT3 box_pos = clamp_float3_box(player_position, m_area_origin_position, m_area_size);

    const float length = DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&player_position), DirectX::XMLoadFloat3(&box_pos))));


    return (length < 3.0f);
}

void Goal_Judge::init_stage_1()
{
    constexpr DirectX::XMFLOAT3 position = { 0.0f,-5.0f,126.0f };

    m_area_origin_position  = position;
    m_area_size             = { 12.0f,0.5f,5.0f };
}

void Goal_Judge::init_stage_2()
{
    constexpr DirectX::XMFLOAT3 position = { 0.0f,8.0f,138.0f };

    m_area_origin_position = position;
    m_area_size             = { 12.0f,0.5f,1.0f };
}

void Goal_Judge::init_stage_3()
{
    constexpr DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 134.0f };

    m_area_origin_position = position;
    m_area_size = { 12.0f,0.5f,1.0f };
}

void Goal_Judge::init_stage_4()
{
    constexpr DirectX::XMFLOAT3 position = { 0.0f, 10.0f,110.0f };

    m_area_origin_position = position;
    m_area_size = { 30.0f,1.0f,10.0f };
}
