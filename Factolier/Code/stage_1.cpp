
#include "stage_1.h"
#include "scene_manager.h"


Stage_1::Stage_1(Scene_Manager* ptr_scene_manager_)
{
    load_model(ptr_scene_manager_->model_manager()->load_model("./Data/stage_demo.fbx", true));
}

void Stage_1::update(const float elapsed_time)
{
    
}
