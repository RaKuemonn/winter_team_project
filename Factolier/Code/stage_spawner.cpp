#include "stage_spawner.h"
#include "stage.h"

#include "stage_1.h"
#include "stage_1_movement.h"

#include "stage_2.h"
#include "stage_2_movement.h"

#include "stage_3.h"
#include "stage_3_movement.h"
#include "stage_3_move.h"

#include "stage_4.h"
#include "stage_4_movement.h"

#include "stage_2_obj.h"
#include "stage_boss.h"


void Stage_Spawner::set_stage_1()
{
    set_stage<Stage_1>();
    set_stage<Stage_1_Movement>();
}

void Stage_Spawner::set_stage_2()
{
    set_stage<Stage_2>();
    set_stage<Stage_2_Movement>();
    set_stage<Stage_2_Obj>();
}

void Stage_Spawner::set_stage_3()
{
    set_stage<Stage_3>();
    set_stage<Stage_3_Movement>();
    set_stage<Stage_3_Move>();
}

void Stage_Spawner::set_stage_4()
{
    set_stage<Stage_4>();
    set_stage<Stage_4_Movement>();
}

void Stage_Spawner::set_stage_boss()
{
    set_stage<Stage_Boss>();
}