#pragma once

#include "model_resource.h"
#include "memory"
#include "DirectXMath.h"


class Model
{
public:

    Model(std::shared_ptr<Model_Resource>);
    ~Model() {}

private:
    std::shared_ptr<Model_Resource> resource;

    Model_Resource::Keyframe now_key;         //現在姿勢
    Model_Resource::Animation now_anime;      //現在アニメーション
    float anime_timer = 0;
    float blend_timer = 0.0f;
    bool blend_mode = false;
    bool anime_play_flag = false;


public:
    void play_animation(float elapsed_time, int anime_num, bool loop = true, float blend_second = 1.0f);
    void append_animation(const char* animation_filename, float sampling_rate = 60.0f) { resource->append_animations(animation_filename, sampling_rate); }
    const bool get_anime_play_flag() const { return anime_play_flag; }

    void render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);
    void render_mesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num);
    void render_exmesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num);

    bool raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4X4& world);
};
