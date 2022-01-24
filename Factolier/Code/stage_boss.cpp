
#include "stage_boss.h"
#include "scene_manager.h"
#include "transform.h"
#include "model_filepaths.h"

Stage_Boss::Stage_Boss(Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position) : Stage(position)
{
    load_model(ptr_scene_manager_->model_manager()->load_model(Model_Paths::Stage::stage_boss, true));

    //get_transform()->set_position({ 0.0f,-10.0f,0.0f });
    get_transform()->set_scale(stage::scale * 50.0f);
    //get_transform()->set_euler({ 0.0f, -3.14f, 0.0f });
    get_transform()->Update();
}

void Stage_Boss::update(const float elapsed_time)
{

}
