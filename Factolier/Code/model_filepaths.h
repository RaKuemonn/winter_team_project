#pragma once

// すべてのモデルのファイルパス
namespace Model_Paths
{
    namespace Entity
    {
        
//#ifdef _DEBUG
        inline constexpr char* const test               = "./Data/nico.fbx";
//#endif


        inline constexpr char* const player             = /*"./Data/Player/player_animation.fbx"*/test;
        inline constexpr char* const ball               = "./Data/ball_demo.fbx";
        inline constexpr char* const enemy_none         = test;
        inline constexpr char* const enemy_move_closer  = test;
        inline constexpr char* const enemy_boss_head    = "./Data/Boss/head/boss_head.fbx";
        inline constexpr char* const enemy_boss_body3   = "./Data/Boss/body3/boss_body3.fbx";
        inline constexpr char* const enemy_boss_body2   = "./Data/Boss/body2/boss_body2.fbx";
        inline constexpr char* const enemy_boss_body1   = "./Data/Boss/body1/boss_body1.fbx";

    };

    namespace Stage
    {
        inline constexpr char* const stage_1    = "./Data/stage1_test.fbx";
        inline constexpr char* const propeller  = "./Data/propeller.fbx";

    };
};
