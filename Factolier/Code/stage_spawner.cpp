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
#include "stage_3_obj.h"

#include "stage_4.h"
#include "stage_4_movement.h"
#include "stage_4_obj.h"

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
    set_stage<Stage_3_Movement>({ 0.0f,-10.0f,0.0f });
    set_stage<Stage_3_Move>({ 0.0f,10.0f,90.0f });
    set_stage<Stage_3_Obj>({ 0.0f,-10.0f,0.0f });
}

void Stage_Spawner::set_stage_4()
{
    set_stage<Stage_4>({ 0.0f,-10.0f,0.0f });
    set_stage<Stage_4_Movement>({ 0.0f,10.0f,90.0f });
    set_stage<Stage_4_Obj>({ 0.0f,-10.0f,0.0f });
}

void Stage_Spawner::set_stage_boss()
{
    set_stage<Stage_Boss>();
}