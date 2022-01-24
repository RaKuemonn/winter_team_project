
#include "stage_3_move.h"
#include "scene_manager.h"
#include "transform.h"
#include "model_filepaths.h"

Stage_3_Move::Stage_3_Move(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position) : Stage(position)
{
    load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Stage::stage_3_move, true));

    get_transform()->set_position({ 0.0f,-10.0f,0.0f });
    get_transform()->set_scale(stage::scale);
    get_transform()->set_euler({ 0.0f, -3.14f, 0.0f });
    get_transform()->Update();

    set_is_raycast(false);
}

void Stage_3_Move::update(const float elapsed_time)
{

}
