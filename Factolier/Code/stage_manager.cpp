
#include "stage_manager.h"
#include "scene_manager.h"
#include "stage.h"
#include "collision.h"

void Stage_Manager::update(const float elapsed_time)
{
    for (auto& stage : vec_registers)
    {
        vec_stages.emplace_back(stage);
    }
    vec_registers.clear();


    for (auto& stage : vec_stages)
    {
        stage->update(elapsed_time);
    }

    for (auto& stage : vec_removes)
    {
        auto it = std::find(vec_stages.begin(), vec_stages.end(), stage);

        if (it != vec_stages.end())
        {
            vec_stages.erase(it);
        }

        if (stage == nullptr) continue;

        stage.reset();
    }
    vec_removes.clear();

}

void Stage_Manager::render(Scene_Manager* scene_manager)
{

    ID3D11DeviceContext* ptr_device_context = scene_manager->device_context();

    // シェーダの設定
    scene_manager->state_manager()->setDS(DS::ON_ON);
    Shader* ptr_shader = scene_manager->shader_manager()->get_shader(Shaders::PHONG);


    // ー　モデルの描画　ー //
    ptr_shader->begin(ptr_device_context);

    for(auto& stage : vec_stages)
    {
        stage->render(ptr_device_context);
    }

    ptr_shader->end(ptr_device_context);
    // ーーーーーーーーーー //
}

void Stage_Manager::all_clear()
{
    vec_removes.clear();
    vec_registers.clear();
    vec_stages.clear();
}

bool Stage_Manager::ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)
{
    bool hit = false;
    Hit_Result result;
    result.distance         = FLT_MAX;
    hit_result_->distance   = FLT_MAX;

    for (auto& stage : vec_stages)
    {
        if (stage->ray_cast(start, end, &result) == false) continue;

        if (result.distance > hit_result_->distance) continue;


        *hit_result_ = result;
        hit = true;
        result.distance = FLT_MAX;
    }

    return hit;
}

void Stage_Manager::spawn_register(std::unique_ptr<Stage>& stage)
{
    vec_registers.emplace_back(std::move(stage));
}

void Stage_Manager::spawn_register(std::unique_ptr<Stage>&& stage)
{
    vec_registers.emplace_back(std::move(stage));
}

void Stage_Manager::spawn_register(std::shared_ptr<Stage>& stage)
{
    vec_registers.emplace_back(stage);
}

void Stage_Manager::remove_register(Stage* stage)
{
    vec_removes.emplace_back(stage);
}
