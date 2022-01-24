#pragma once

#include "stage.h"

class Stage_4_Move final : public Stage
{
public:
    Stage_4_Move(class Scene_Manager* ptr_scene_manager_, const DirectX::XMFLOAT3& position = {});
    ~Stage_4_Move() override = default;

    void update(const float elapsed_time)               override;
    bool ray_cast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, Hit_Result* hit_result_)    override;

    Stage_4_Move& set_start(const DirectX::XMFLOAT3& start_) { start = start_; return *this; };
    Stage_4_Move& set_end(const DirectX::XMFLOAT3& end_) { end = end_; return *this; };
    Stage_4_Move& set_scale(const float scale) { get_transform()->set_scale({scale,scale,scale}); return *this; };

private:
    DirectX::XMFLOAT3 start = {};
    DirectX::XMFLOAT3 end = {};
    float move_ratio = 1.0f;
    float move_rate_ = 0.5f;
    float move_speed = 7.5f;

    DirectX::XMFLOAT4X4 old_transform = {};
    DirectX::XMFLOAT3 m_euler = { 0,0,0 };
    DirectX::XMFLOAT3 old_euler = { 0,0,0 };
};