#pragma once

#include "stage.h"

class Stage_3_Move final : public Stage
{
public:
    Stage_3_Move(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_3_Move() override = default;

    void update(const float elapsed_time)               override;
    bool ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)    override;

    Stage_3_Move& set_start(const DirectX::XMFLOAT3& start_) { start = start_; return *this; };
    Stage_3_Move& set_end(const DirectX::XMFLOAT3& end_) { end = end_; return *this;};

private:
    DirectX::XMFLOAT3 start = {};
    DirectX::XMFLOAT3 end   = {};
    float move_ratio = 1.0f;
    float move_rate_ = 0.5f;
    float move_speed = 7.5f;

    DirectX::XMFLOAT4X4 old_transform = {};
    DirectX::XMFLOAT3 m_euler = { 0,0,0 };
    DirectX::XMFLOAT3 old_euler = { 0,0,0 };
};