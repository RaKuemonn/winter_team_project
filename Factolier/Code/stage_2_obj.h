#pragma once

#include "stage.h"

class Stage_2_Obj final : public Stage
{
public:
    Stage_2_Obj(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_2_Obj() override = default;

    void update(const float elapsed_time)               override;

};