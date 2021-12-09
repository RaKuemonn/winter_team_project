#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>
#include <filesystem>

#include <cereal\archives\binary.hpp>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>



static const int MAX_BONE_INFLUENCES{ 4 };
static const int MAX_BONES{ 256 };



namespace DirectX
{
    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT2& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT3& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4& v)
    {
        archive(
            cereal::make_nvp("x", v.x),
            cereal::make_nvp("y", v.y),
            cereal::make_nvp("z", v.z),
            cereal::make_nvp("w", v.w)
        );
    }

    template<class T>
    void serialize(T& archive, DirectX::XMFLOAT4X4& m)
    {
        archive(
            cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12),
            cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
            cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22),
            cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
            cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32),
            cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
            cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42),
            cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
        );
    }
}



class Skinned_Mesh
{
public:
    struct Node
    {
        uint64_t                                    unique_id{ 0 };
        std::string                                 name;
        std::vector<FbxNodeAttribute::EType>        attribute{};
        int64_t                                     parent_index{ -1 };

        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, attribute, parent_index);
        }
    };


    struct Vertex
    {
        DirectX::XMFLOAT3   position;
        DirectX::XMFLOAT3   normal;
        DirectX::XMFLOAT4   tangent;
        DirectX::XMFLOAT2   texcoord;
        float               bone_weights[MAX_BONE_INFLUENCES]{ 1, 0, 0, 0 };
        uint32_t            bone_indices[MAX_BONE_INFLUENCES]{};

        template<class T>
        void serialize(T& archive)
        {
            archive(position, normal, tangent, texcoord, bone_weights, bone_indices);
        }
    };


    struct Material
    {
        DirectX::XMFLOAT4                                   color = { 1.0f, 1.0f, 1.0f, 1.0f };
        float                                               factor{ 1 };
        std::string                                         texture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    shader_resource_view;

        template<class T>
        void serialize(T& archive)
        {
            archive(color, factor, texture);
        }

    };


    struct Subset
    {
        std::string         name;
        unsigned int        index_start = 0; // start number of index buffer
        unsigned int        index_count = 0; // number of vertices (indices)
        int                 material_index{ -1 };

        Material            ambient;
        Material            diffuse;
        Material            emissive;
        Material            specular;
        Material            bump;
        Material            normal_map;

        //pbrƒ}ƒeƒŠƒAƒ‹
        Material            albedo;
        Material            metalness;
        Material            roughness;


        Subset() = default;

        template<class T>
        void serialize(T& archive)
        {
            archive(name, index_start, index_count, material_index, ambient, diffuse, emissive, specular, bump, normal_map, albedo, metalness, roughness);
        }
    };


    struct Bone
    {
        uint64_t        unique_id{ 0 };
        std::string     name;
        int64_t         parent_index{ -1 }; // -1 : the bone is orphan
        int64_t         node_index{ 0 };

        // 'offset_transform' is used to convert from model(mesh) space to bone(node) scene.
        DirectX::XMFLOAT4X4 offset_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, parent_index, node_index, offset_transform);
        }

        bool is_orphan() const { return parent_index < 0; };
    };


    struct Skeleton
    {
        std::vector<Bone> bones;

        template<class T>
        void serialize(T& archive)
        {
            archive(bones);
        }

        int64_t indexof_born(uint64_t unique_id) const
        {
            int64_t index{ 0 };
            for (const Bone& bone : bones)
            {
                if (bone.unique_id == unique_id)
                {
                    return index;
                }
                ++index;
            }
            return -1;
        }
    };


    struct Bone_Influence
    {
        uint32_t bone_index;
        float bone_weight;
    };
    using bone_influences_per_control_point = std::vector<Bone_Influence>;


    struct MeshData
    {
        uint64_t                unique_id{ 0 };
        std::string             name;
        int64_t                 node_index{ 0 };    //'node_index' is an index that refers to the node array of the scene
        std::vector<Vertex>     vertices;
        std::vector<uint32_t>   indices;
        std::vector<Subset>     subsets;
        Skeleton                skeletons;
        DirectX::XMFLOAT4X4     default_global_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        DirectX::XMFLOAT3       bounding_box[2]
        {
            { +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX },
            { -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX }
        };

        Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;

        template<class T>
        void serialize(T& archive)
        {
            archive(unique_id, name, node_index, vertices, indices, subsets, skeletons, default_global_transform, bounding_box);
        }
    };


    struct Constants
    {
        DirectX::XMFLOAT4X4     world;
        DirectX::XMFLOAT4       material_color;
        DirectX::XMFLOAT4X4     bone_transforms[MAX_BONES]{ { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 } };
    };


    struct NodeKeyData
    {
        // 'global_transform' is used to convert from local space of node to global space of scene.
        DirectX::XMFLOAT4X4     global_transform{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        // The transformation data of a node includes its translation, rotation and scaling vectors
        // with respect to its parent.
        DirectX::XMFLOAT3       scaling{ 1, 1, 1 };
        DirectX::XMFLOAT4       rotation{ 0, 0, 0, 1 }; // Rotation quaternion
        DirectX::XMFLOAT3       translation{ 0, 0, 0 };

        template<class T>
        void serialize(T& archive)
        {
            archive(global_transform, scaling, rotation, translation);
        }
    };


    struct Keyframe
    {
        float						seconds;
        std::vector<NodeKeyData>	node_keys;

        template<class T>
        void serialize(T& archive)
        {
            archive(seconds, node_keys);
        }
    };


    struct Animation
    {
        std::string					name;
        float						sampling_rate;
        std::vector<Keyframe>		keyframes;

        template<class T>
        void serialize(T& archive)
        {
            archive(name, sampling_rate, keyframes);
        }
    };


private:
    Microsoft::WRL::ComPtr<ID3D11Buffer>            constant_buffer;

public:
    Skinned_Mesh(ID3D11Device* device, const char* fbx_filename, bool triangulate = false, float rate = 60.0f);
    virtual ~Skinned_Mesh() = default;

    void fetch_meshes(FbxScene* fbx_scene, const char* filename);
    void fetch_material(const FbxSurfaceMaterial* fbx_material, Material& material, const char* propertyName, const char* factorName, const char* filename);
    void fetch_pbr(const char* filename, std::string mesh_name, Material& material, const char* pbr_name);
    void fetch_skeleton(FbxMesh* fbx_mesh, Skeleton& skeleton);
    void fetch_bone_influences(const FbxMesh* fbx_mesh, std::vector<bone_influences_per_control_point>& bone_influences);
    void fetch_animations(FbxScene* fbx_scene, float sampling_rate /*If this value is 0, the animation data will be sampled at the default frame rate.*/);

    void update_animation(Keyframe& keyframe);
    bool append_animations(const char* animation_filename, float sampling_rate);
    void blend_animations(const Keyframe* keyframes[2], float factor, Keyframe& keyframe);

    void create_com_objects(ID3D11Device* device, const char* fbx_filename);
    HRESULT make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view, DWORD value/*0xAABBGGRR*/, UINT dimension);

    void render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, const Keyframe* keyframe);
    void render_mesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, const Keyframe* keyframe, int mesh_num);
    void render_exmesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, const Keyframe* keyframe, int mesh_num);


    std::vector<Node>       nodes;
    std::vector<MeshData>	meshes;
    std::vector<Animation>	animations;
    bool                    pbr{ false };
    std::filesystem::path   fcm_filename;

protected:
    int64_t indexof_node(uint64_t unique_id) const
    {
        int64_t index{ 0 };
        for (const Node& node : nodes)
        {
            if (node.unique_id == unique_id)
            {
                return index;
            }

            ++index;
        }

        return -1;
    }
};
