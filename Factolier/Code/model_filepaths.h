#pragma once

// すべてのモデルのファイルパス
namespace Model_Paths
{
    namespace Entity
    {
        
//#ifdef _DEBUG
        inline constexpr char* const test               = "./Data/Model/nico.fbx";
//#endif


        inline constexpr char* const player             = /*"./Data/Model/Player/player_animation.fbx"*/test;
        inline constexpr char* const ball               = "./Data/Model/ball.fbx";
        //inline constexpr char* const ball               = "./Data/ball.fbx";
        inline constexpr char* const enemy_none         = test;
        inline constexpr char* const enemy_move_closer  = test;
        inline constexpr char* const enemy_boss_head    = "./Data/Model/Boss/head/boss_head.fbx";
        inline constexpr char* const enemy_boss_body3   = "./Data/Model/Boss/body3/boss_body3.fbx";
        inline constexpr char* const enemy_boss_body2   = "./Data/Model/Boss/body2/boss_body2.fbx";
        inline constexpr char* const enemy_boss_body1   = "./Data/Model/Boss/body1/boss_body1.fbx";

    };

    namespace Stage
    {
        //inline constexpr char* const stage_1    = "./Data/stage1_test.fbx";
        inline constexpr char* const stage_1            = "./Data/Model/stage1/stage1.fbx";
        inline constexpr char* const stage_1_propeller  = "./Data/Model/stage1/stage1_propeller.fbx";

        inline constexpr char* const stage_2            = "./Data/Model/stage2/stage2.fbx";
        inline constexpr char* const stage_2_propeller  = "./Data/Model/stage2/stage2_propeller.fbx";

        inline constexpr char* const stage_3            = "./Data/Model/stage1/stage1.fbx";
        inline constexpr char* const stage_4            = "./Data/Model/stage1/stage1.fbx";
        inline constexpr char* const stage_boss         = "./Data/Model/boss_stage/boss_stage.fbx";

    };
};
