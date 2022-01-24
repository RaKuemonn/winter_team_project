#pragma once

#include "stage.h"

class Stage_4_Goal final : public Stage
{
public:
    Stage_4_Goal(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_4_Goal() override = default;

    void update(const float elapsed_time)               override;

};