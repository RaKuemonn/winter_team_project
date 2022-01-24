#pragma once

#include "stage.h"

class Stage_2 final : public Stage
{
public:
    Stage_2(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_2() override = default;

    void update(const float elapsed_time)               override;

};