#pragma once
#include <cassert>
#include <DirectXMath.h>
#include <memory>
#include "stage_manager.h"

namespace stage_spawner_template
{
    template <typename T>
    constexpr bool false_v = false;
};


class Stage_Spawner final
{
public:
    Stage_Spawner(class Scene_Manager* ptr_scene_manager_) : ptr_scene_manager(ptr_scene_manager_) {};
    ~Stage_Spawner() = default;

    void set_stage_1();
    void set_stage_2();
    void set_stage_3();
    void set_stage_4();
    void set_stage_boss();



private:
    template<typename T>
    Stage_Spawner& set_stage(const DirectX::XMFLOAT3 & position_ = {})
    {

        std::unique_ptr<Stage> stage;

        if constexpr (std::is_base_of<Stage, T>{})
        {
            stage = std::make_unique<T>(ptr_scene_manager);
        }

        else
        {
            // 和訳 : この型はサポートされてないよ！！　せめてStageを継承している型を使用してね！
            static_assert(stage_spawner_template::false_v<T>, "This type is not supported.  Must be a type that inherits from Stage.");
        }


        assert(stage.get());

        //stage->set_position(position_);

        Stage_Manager::instance().spawn_register(stage);

        return *this;
    }


private:
    class Scene_Manager* ptr_scene_manager = nullptr;
};

