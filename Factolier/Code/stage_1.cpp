
#include "stage_1.h"
#include "scene_manager.h"
#include "transform.h"
#include "model_filepaths.h"

Stage_1::Stage_1(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position) : Stage(position)
{
    load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Stage::stage_1, true));

    get_transform()->set_position({ 0.0f,-10.0f,0.0f });
    get_transform()->set_scale(stage::scale);
    get_transform()->set_euler({ 0.0f, -3.14f, 0.0f });
    get_transform()->Update();
}

void Stage_1::update(const float elapsed_time)
{
    
}
