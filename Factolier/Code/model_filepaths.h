#pragma once

// すべてのモデルのファイルパス
namespace Model_Paths
{
    namespace Entity
    {
        
//#ifdef _DEBUG
        inline constexpr char* const test               = "./Data/Model/nico.fbx";
//#endif


        //inline constexpr char* const player             = "./Data/Model/Player/character.fbx";
        //inline constexpr char* const player_animation   = "./Data/Model/Player/player_animation.fbx";
        inline constexpr char* const player             = "./Data/Model/Player/player_animation.fbx";
        //inline constexpr char* const player             = test;
        inline constexpr char* const ball               = "./Data/Model/ball.fbx";
        inline constexpr char* const enemy_none         = test;
        inline constexpr char* const enemy_move_closer  = test;

        inline constexpr char* const enemy_move_spring  = "./Data/Model/Enemy/enemy1_spring_animation.fbx";
        inline constexpr char* const enemy_move_summer  = "./Data/Model/Enemy/enemy1_summer_animation.fbx";
        inline constexpr char* const enemy_move_fall    = "./Data/Model/Enemy/enemy1_fall_animation.fbx";
        inline constexpr char* const enemy_move_winter  = "./Data/Model/Enemy/enemy1_winter_animation.fbx";
        inline constexpr char* const enemy_rock         = "./Data/Model/Enemy/enemy2_animation.fbx";

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
        inline constexpr char* const stage_1_obj        = "./Data/Model/stage1/stage1_obj.fbx";

        inline constexpr char* const stage_2            = "./Data/Model/stage2/stage2.fbx";
        inline constexpr char* const stage_2_propeller  = "./Data/Model/stage2/stage2_propeller.fbx";
        inline constexpr char* const stage_2_obj        = "./Data/Model/stage2/stage2_obj.fbx";

        inline constexpr char* const stage_3            = "./Data/Model/stage3/stage3.fbx";
        inline constexpr char* const stage_3_propeller  = "./Data/Model/stage3/stage3_propeller.fbx";
        inline constexpr char* const stage_3_move       = "./Data/Model/stage3/stage3_move.fbx";
        inline constexpr char* const stage_3_goal       = "./Data/Model/stage3/stage3_goal.fbx";


        inline constexpr char* const stage_4            = "./Data/Model/stage4/stage4.fbx";
        inline constexpr char* const stage_4_propeller  = "./Data/Model/stage4/stage4_propeller.fbx";
        inline constexpr char* const stage_4_goal       = "./Data/Model/stage4/stage4_goal.fbx";
        //inline constexpr char* const stage_4_obj        = "./Data/Model/stage4/stage4_obj.fbx";

        inline constexpr char* const stage_boss         = "./Data/Model/boss_stage/boss_stage.fbx";

    };
};
