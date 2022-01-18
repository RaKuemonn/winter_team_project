#pragma once

// すべてのモデルのファイルパス
namespace Model_Paths
{
    namespace Entity
    {
        
//#ifdef _DEBUG
        inline constexpr char* const test               = "./Data/nico.fbx";
//#endif


        inline constexpr char* const player             = test;
        inline constexpr char* const ball               = "./Data/ball_demo.fbx";
        inline constexpr char* const enemy_none         = test;
        inline constexpr char* const enemy_move_closer  = test;

    };

    namespace Stage
    {
        inline constexpr char* const stage_1    = "./Data/stage1_test.fbx";
        inline constexpr char* const propeller  = "./Data/propeller.fbx";

    };
};
