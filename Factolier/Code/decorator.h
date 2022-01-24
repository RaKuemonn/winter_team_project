#pragma once


#include "stage.h"


namespace Model_Paths
{
    namespace Deco
    {
        inline constexpr char* const egg            = "./Data/Model/stage1/obj/stage1_egg.fbx";
        inline constexpr char* const egg_2          = "./Data/Model/stage1/obj/stage1_egg_2.fbx";

        inline constexpr char* const ball           = "./Data/Model/stage2/obj/stage2_float_and_ball.fbx";
        inline constexpr char* const tetrapod       = "./Data/Model/stage2/obj/stage2_Tetrapod.fbx";
        inline constexpr char* const umbrella_and_bed = "./Data/Model/stage2/obj/stage2_umbrella_and_bed.fbx";


        inline constexpr char* const kinoko     = "./Data/Model/stage3/object/stage3_kinoko.fbx";
        inline constexpr char* const aki_leaf   = "./Data/Model/stage3/object/stage3_leaf.fbx";
        inline constexpr char* const pumpkin    = "./Data/Model/stage3/object/stage3_pumpkin.fbx";

        inline constexpr char* const kamakura   = "./Data/Model/stage4/object/stage4_kamakura.fbx";
        inline constexpr char* const fuyu_leaf  = "./Data/Model/stage4/object/stage4_leaf.fbx";
        inline constexpr char* const snow_man   = "./Data/Model/stage4/object/stage4_snowman.fbx";
    };
};


class Decorator : public Stage
{
public:
    Decorator()
    {
        get_transform()->set_scale(stage::scale);
        set_is_raycast(false);
    };
    virtual ~Decorator()override = default;

    void update(const float elapsed_time) override { get_transform()->Update(); };

    Decorator& set_scale(float scale)
    {
        get_transform()->set_scale({ scale,scale,scale });
        return *this;
    }
};