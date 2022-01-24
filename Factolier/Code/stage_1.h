#pragma once

#include "stage.h"

class Stage_1 final : public Stage
{
public:
    Stage_1(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_1() override = default;

    void update(const float elapsed_time)               override;

};