#pragma once

#include "stage.h"

class Stage_Boss final : public Stage
{
public:
    Stage_Boss(class Scene_Manager* ptr_scene_manager_);
    ~Stage_Boss() override = default;

    void update(const float elapsed_time)               override;

};