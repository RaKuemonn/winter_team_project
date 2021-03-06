#pragma once
#include "stage.h"

class Stage_2_Movement :  public Stage
{
public:
    Stage_2_Movement(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_2_Movement()override = default;

    void update(const float elapsed_time)                                                                   override;
    bool ray_cast(const DirectX::XMFLOAT3 & start, const DirectX::XMFLOAT3 & end, Hit_Result * hit_result_)    override;

private:
    DirectX::XMFLOAT4X4 old_transform = {};
    DirectX::XMFLOAT3 m_euler = { 0,0,0 };
    DirectX::XMFLOAT3 old_euler = { 0,0,0 };
};

