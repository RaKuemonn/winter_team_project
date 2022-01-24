#pragma once

#include "stage.h"

class Stage_4_Obj final : public Stage
{
public:
    Stage_4_Obj(class Scene_Manager* ptr_scene_manager_);
    ~Stage_4_Obj() override = default;

    void update(const float elapsed_time)               override;

};