#include "stage_spawner.h"
#include "stage.h"

#include "stage_1.h"
#include "stage_1_movement.h"
#include "stage_1_obj.h"

#include "stage_2.h"
#include "stage_2_movement.h"
#include "stage_2_obj.h"

#include "stage_3.h"
#include "stage_3_movement.h"
#include "stage_3_move.h"
#include "stage_3_goal.h"

#include "stage_4.h"
#include "stage_4_movement.h"
#include "stage_4_move.h"
#include "stage_4_goal.h"

#include "stage_boss.h"


void Stage_Spawner::set_stage_1()
{
    set_stage<Stage_1>({ 0.0f,-10.0f,0.0f });
    set_stage<Stage_1_Movement>({ 0.0f,-3.0f,50.0f });
    set_stage<Stage_1_Obj>({ 0.0f,-10.0f,0.0f });
}

void Stage_Spawner::set_stage_2()
{
    set_stage<Stage_2>({ 0.0f,0.0f,0.0f });
    set_stage<Stage_2_Movement>({ 0.0f,10.0f,90.0f });
    set_stage<Stage_2_Obj>({ 0.0f,0.0f,0.0f });
}

void Stage_Spawner::set_stage_3()
{
    set_stage<Stage_3>({ 0.0f,-10.0f,0.0f });
    set_stage<Stage_3_Movement>({ 0.0f, -8.0f, -7.0f });
    set_stage<Stage_3_Move>({ 0.0f, -8.0f,-76.0f });
    set_stage<Stage_3_Move>({ 0.0f, -4.0f, 75.0f });
    set_stage<Stage_3_Move>({ 0.0f, -2.0f, 109.0f })->set_start({-15.0f,-2.0f,109.0f}).set_end({ 15.0f,-2.0f,109.0f });
    set_stage<Stage_3_Goal>({ -29.0f, 0.0f, 154.0f });
}

void Stage_Spawner::set_stage_4()
{
    set_stage<Stage_4>({ 0.0f,0.0f,0.0f });
    set_stage<Stage_4_Move>({ 0.0f,1.0f,-67.0f })->set_start({ 0.0f,7.0f,-67.0f }).set_end({ 0.0f,0.0f,-67.0f }).set_scale(0.1f);
    set_stage<Stage_4_Movement>({ 0.0f,5.0f,-15.0f });
    set_stage<Stage_4_Movement>({ 0.0f,2.0f,14.0f });
    set_stage<Stage_4_Goal>({ 0.0f,0.0f,0.0f });
}

void Stage_Spawner::set_stage_boss()
{
    set_stage<Stage_Boss>();
}