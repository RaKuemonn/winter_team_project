#pragma once

#include "stage.h"

class Stage_4 final : public Stage
{
public:
    Stage_4(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_4() override = default;

    void update(const float elapsed_time)               override;

};