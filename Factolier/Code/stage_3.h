#pragma once

#include "stage.h"

class Stage_3 final : public Stage
{
public:
    Stage_3(class Scene_Manager* ptr_scene_manager_);
    ~Stage_3() override = default;

    void update(const float elapsed_time)               override;

};