#pragma once

#include "skinned_mesh.h"
#include "memory"
#include "DirectXMath.h"


class Model
{
public:

    Model(std::shared_ptr<Skinned_Mesh>, bool triangulate = false, float rate = 60.0f);
    ~Model();

private:
    std::shared_ptr<Skinned_Mesh> resource;

    Skinned_Mesh::Keyframe now_key;         //���ݎp��
    Skinned_Mesh::Animation now_anime;      //���݃A�j���[�V����
    float anime_timer = 0;
    float blend_timer = 0.0f;
    bool blend_mode = false;
    bool anime_play_flag = false;

    DirectX::XMFLOAT4X4 world_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

public:
    void play_animation(float elapsed_time, int anime_num, bool loop = true, float blend_second = 1.0f);
    const bool get_anime_play_flag() const { return anime_play_flag; }

    void render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);
    void render_mesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num);
    void render_exmesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num);

    void set_transform(DirectX::XMFLOAT4X4 world) { world_transform = world; }
    bool raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end);
};