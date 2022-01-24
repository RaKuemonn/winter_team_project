
#include "stage_2_obj.h"
#include "scene_manager.h"
#include "transform.h"
#include "model_filepaths.h"

Stage_2_Obj::Stage_2_Obj(Scene_Manager* ptr_scene_manager_)
{
    load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Stage::stage_2_obj, true));

    get_transform()->set_position({ 0.0f,-10.0f,0.0f });
    get_transform()->set_scale(stage::scale);
    get_transform()->set_euler({ 0.0f, -3.14f, 0.0f });
    get_transform()->Update();

    set_is_raycast(false);
}

void Stage_2_Obj::update(const float elapsed_time)
{

}
