
#include "stage_1.h"
#include "scene_manager.h"
#include "transform.h"


Stage_1::Stage_1(Scene_Manager* ptr_scene_manager_)
{
    load_model(ptr_scene_manager_->model_manager()->load_model("./Data/stage1_tentative.fbx", true));

    get_transform()->set_position({ 0.0f,-100.0f,0.0f });
    get_transform()->set_euler({ 0.0f, -3.14f, 0.0f });
    get_transform()->Update();
}

void Stage_1::update(const float elapsed_time)
{
    
}
