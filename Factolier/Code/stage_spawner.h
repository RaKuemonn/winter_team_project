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
    std::shared_ptr<T> set_stage(const DirectX::XMFLOAT3 & position_ = {})
    {

        std::shared_ptr<T> stage;

        if constexpr (std::is_base_of<Stage, T>{})
        {
            stage = std::make_shared<T>(ptr_scene_manager, position_);
        }

        else
        {
            // �a�� : ���̌^�̓T�|�[�g����ĂȂ���I�I�@���߂�Stage���p�����Ă���^���g�p���ĂˁI
            static_assert(stage_spawner_template::false_v<T>, "This type is not supported.  Must be a type that inherits from Stage.");
        }


        assert(stage.get());

        std::shared_ptr<Stage> s_stage = stage;

        Stage_Manager::instance().spawn_register(s_stage);

        return stage;
    }


private:
    class Scene_Manager* ptr_scene_manager = nullptr;
};

