
#include "stage_manager.h"
#include "scene_manager.h"
#include "stage.h"
#include "collision.h"
#include "imgui.h"

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

        if (stage.get() == nullptr) continue;
        stage.reset();
    }
    vec_removes.clear();

}

void Stage_Manager::render(ID3D11DeviceContext* ptr_device_context)
{
    for(auto& stage : vec_stages)
    {
        stage->render(ptr_device_context);
    }
}

void Stage_Manager::all_clear()
{

#ifdef _DEBUG
    file_write();
#endif


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
        if(stage->get_is_raycast() == false) continue;

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
    //auto it = std::find(vec_stages.begin(), vec_stages.end(), stage);
    //
    //if (it == vec_stages.end()) return;
    //
    //vec_removes.emplace_back(*it);

    vec_removes.emplace_back(get_stage(stage));
}

std::shared_ptr<Stage> Stage_Manager::get_stage(Stage* entity_) const
{
    //auto it = std::find(vec_entities.begin(), vec_entities.end(), entity_);
    //return *it;

    for (auto& stage : vec_stages)
    {
        if (stage.get() != entity_) continue;

        return stage;
    }

    return nullptr;
}

void Stage_Manager::imgui()
{
#ifdef _DEBUG
    ImGui::Begin("stage_manager");

    DirectX::XMFLOAT3 position = {};
    int i = 0;
    for(auto& stage : vec_stages)
    {
        position = stage->get_position();
        ImGui::InputFloat3("position", &position.x);

        bool is_check = false;
        std::string mozi = "remove" + std::to_string(i);
        ImGui::Checkbox(mozi.c_str(), &is_check);
        if (is_check)
        {
            remove_register(stage.get());
        }

        i++;
    }


    ImGui::End();
#endif

}