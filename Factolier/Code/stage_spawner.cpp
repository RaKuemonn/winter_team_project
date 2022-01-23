#include "stage_spawner.h"
#include "stage.h"

#include "stage_1.h"
#include "stage_1_movement.h"

#include "stage_2.h"
#include "stage_2_movement.h"


void Stage_Spawner::set_stage_1()
{
    set_stage<Stage_1>();
    set_stage<Stage_1_Movement>();
}

void Stage_Spawner::set_stage_2()
{
    set_stage<Stage_2>();
    set_stage<Stage_2_Movement>();
}

void Stage_Spawner::set_stage_3()
{

}

void Stage_Spawner::set_stage_4()
{

}

void Stage_Spawner::set_stage_boss()
{

}