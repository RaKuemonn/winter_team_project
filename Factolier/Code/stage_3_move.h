#pragma once

#include "stage.h"

class Stage_3_Move final : public Stage
{
public:
    Stage_3_Move(class Scene_Manager* ptr_scene_manager_);
    ~Stage_3_Move() override = default;

    void update(const float elapsed_time)               override;

};