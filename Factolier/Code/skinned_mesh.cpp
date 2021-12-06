
#include "misc.h"
#include "skinned_mesh.h"
#include <fstream>
#include <functional>
#include <filesystem>
#include "shader.h"
#include "texture.h"

using namespace DirectX;



Skinned_Mesh::Skinned_Mesh(ID3D11Device* device, const char* fbx_filename, bool triangulate, float rate)
{
    //ファイルパス検索
    std::filesystem::path cereal_filename(fbx_filename);
    cereal_filename.replace_extension("fcm");

    if (std::filesystem::exists(cereal_filename.c_str()))
    {
        //シリアライズデータ読み込み
        std::ifstream ifs(cereal_filename.c_str(), std::ios::binary);
        cereal::BinaryInputArchive deserialization(ifs);
        deserialization(nodes, meshes, animations);
    }

    else
    {
        //マネージャー作成
        FbxManager* fbx_manager{ FbxManager::Create() };
        FbxScene* fbx_scene{ FbxScene::Create(fbx_manager, "") };


        //インポーター作成
        FbxImporter* fbx_importer{ FbxImporter::Create(fbx_manager, "") };
        bool import_status{ false };
        import_status = fbx_importer->Initialize(fbx_filename);     //ファイルをインポート
        _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

        import_status = fbx_importer->Import(fbx_scene);
        _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

        FbxGeometryConverter fbx_converter(fbx_manager);


        //三角化
        if (triangulate)
        {
            fbx_converter.Triangulate(fbx_scene, true/*replace*/, false/*legacy*/);
            fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);
        }


        //ノード取得
        std::function<void(FbxNode*)> traverse{ [&](FbxNode* fbx_node)
            {
                Node& node{ nodes.emplace_back() };
                //全てのアトリビュートを取得
                for (int i = 0; i < fbx_node->GetNodeAttributeCount(); i++)
                {
                    node.attribute.emplace_back(fbx_node->GetNodeAttributeByIndex(i)->GetAttributeType());
                }
                node.name = fbx_node->GetName();
                node.unique_id = fbx_node->GetUniqueID();
                node.parent_index = indexof_node(fbx_node->GetParent() ?
                    fbx_node->GetParent()->GetUniqueID() : 0);

                for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index)
                {
                    //再起処理
                    traverse(fbx_node->GetChild(child_index));
                }
            }
        };
        traverse(fbx_scene->GetRootNode());


        //メッシュのフェッチ
        fetch_meshes(fbx_scene, fbx_filename);

        //アニメーションのフェッチ
        float sampling_rate = rate;
        fetch_animations(fbx_scene, sampling_rate);

        //マネージャー破棄
        fbx_manager->Destroy();

        //シリアライズデータ作成
        std::ofstream ofs(cereal_filename.c_str(), std::ios::binary);
        cereal::BinaryOutputArchive serialization(ofs);
        serialization(nodes, meshes, animations);
    }

    create_com_objects(device, fbx_filename);
}


inline XMFLOAT4X4 to_xmfloat4x4(const FbxAMatrix& fbxamatrix)
{
    XMFLOAT4X4 xmfloat4x4;

    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
        }
    }

    return xmfloat4x4;
}


inline XMFLOAT3 to_xmfloat3(const FbxDouble3& fbxdouble3)
{
    XMFLOAT3 xmfloat3;
    xmfloat3.x = static_cast<float>(fbxdouble3[0]);
    xmfloat3.y = static_cast<float>(fbxdouble3[1]);
    xmfloat3.z = static_cast<float>(fbxdouble3[2]);

    return xmfloat3;
}


inline XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
{
    XMFLOAT4 xmfloat4;
    xmfloat4.x = static_cast<float>(fbxdouble4[0]);
    xmfloat4.y = static_cast<float>(fbxdouble4[1]);
    xmfloat4.z = static_cast<float>(fbxdouble4[2]);
    xmfloat4.w = static_cast<float>(fbxdouble4[3]);

    return xmfloat4;
}


void Skinned_Mesh::fetch_meshes(FbxScene* fbx_scene, const char* filename)
{
    //メッシュのノード取得
    for (const Node& node : nodes)
    {
        for (int i = 0; i < node.attribute.size(); i++)
        {
            //ノードのアトリビュートがメッシュ以外なら飛ばす
            if (node.attribute[i] != FbxNodeAttribute::EType::eMesh) continue;


            //ノードとメッシュのデータ取得
            FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };
            FbxMesh* fbx_mesh{ fbx_node->GetMesh() };


            //メッシュの情報を記録
            MeshData& mesh{ meshes.emplace_back() };
            mesh.unique_id = fbx_mesh->GetNode()->GetUniqueID();
            mesh.name = fbx_mesh->GetNode()->GetName();
            mesh.node_index = indexof_node(mesh.unique_id);
            mesh.default_global_transform = to_xmfloat4x4(fbx_mesh->GetNode()->EvaluateGlobalTransform());


            //ボーンのウエイト値を取得
            std::vector<bone_influences_per_control_point> bone_influences;
            fetch_bone_influences(fbx_mesh, bone_influences);

            //バインドポーズを取得
            fetch_skeleton(fbx_mesh, mesh.skeletons);


            //全てのマテリアルの情報を取得

            const int material_count{ fbx_mesh->GetNode()->GetMaterialCount() };
            mesh.subsets.resize(material_count > 0 ? material_count : 1);

            //物理ベーステクスチャの読み込み

            if (pbr)
            {
                fetch_pbr(filename, mesh.name, mesh.subsets.at(0).albedo, "albedo");
                fetch_pbr(filename, mesh.name, mesh.subsets.at(0).metalness, "metalness");
                fetch_pbr(filename, mesh.name, mesh.subsets.at(0).roughness, "roughness");
            }

            //通常テクスチャ
            for (int material_index = 0; material_index < material_count; ++material_index)
            {
                Subset& subset = mesh.subsets.at(material_index);
                subset.material_index = material_index;

                const FbxSurfaceMaterial* fbx_material{ fbx_mesh->GetNode()->GetMaterial(material_index) };


                fetch_material(fbx_material, subset.ambient, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, filename);
                fetch_material(fbx_material, subset.diffuse, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, filename);
                fetch_material(fbx_material, subset.emissive, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, filename);
                fetch_material(fbx_material, subset.bump, FbxSurfaceMaterial::sBump, FbxSurfaceMaterial::sBumpFactor, filename);
                fetch_material(fbx_material, subset.normal_map, FbxSurfaceMaterial::sNormalMap, FbxSurfaceMaterial::sBumpFactor, filename);

                fetch_material(fbx_material, subset.specular, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, filename);

                subset.name = fbx_material->GetName();

                //マテリアルがない場合、ダミーのマテリアル追加
                if (material_count == 0)
                {
                    for (MeshData& mesh : meshes)
                    {
                        mesh.subsets.emplace_back(Subset{ mesh.name });
                    }
                }
            }
            

            if (material_count > 0)
            {
                //ポリゴン設定
                const int polygon_count{ fbx_mesh->GetPolygonCount() };

                for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
                {
                    const int material_index{ fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) };
                    mesh.subsets.at(material_index).index_count += 3;
                }
                uint32_t offset{ 0 };

                for (Subset& subset : mesh.subsets)
                {
                    subset.index_start = offset;
                    offset += subset.index_count;
                    // This will be used as counter in the following procedures, reset to zero
                    subset.index_count = 0;
                }
            }


            //ポリゴン設定
            const int polygon_count{ fbx_mesh->GetPolygonCount() };
            mesh.vertices.resize(polygon_count * 3LL);
            mesh.indices.resize(polygon_count * 3LL);


            FbxStringList uv_names;
            fbx_mesh->GetUVSetNames(uv_names);
            const FbxVector4* control_points{ fbx_mesh->GetControlPoints() };

            //ポリゴン取得
            for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
            {
                const int material_index{ material_count > 0 ? fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) : 0 };
                Subset& subset{ mesh.subsets.at(material_index) };
                const uint32_t offset{ subset.index_start + subset.index_count };

                for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon)
                {
                    const int vertex_index{ polygon_index * 3 + position_in_polygon };

                    //頂点情報の取得
                    Vertex vertex;
                    const int polygon_vertex{ fbx_mesh->GetPolygonVertex(polygon_index, position_in_polygon) };
                    vertex.position.x = static_cast<float>(control_points[polygon_vertex][0]);
                    vertex.position.y = static_cast<float>(control_points[polygon_vertex][1]);
                    vertex.position.z = static_cast<float>(control_points[polygon_vertex][2]);


                    //ボーンの影響度の取得
                    const bone_influences_per_control_point& influences_per_control_point{ bone_influences.at(polygon_vertex) };
                    size_t influence_index;
                    for (influence_index = 0; influence_index < influences_per_control_point.size(); ++influence_index)
                    {
                        if (influence_index <= MAX_BONE_INFLUENCES)
                        {
                            vertex.bone_weights[influence_index] = influences_per_control_point.at(influence_index).bone_weight;
                            vertex.bone_indices[influence_index] = influences_per_control_point.at(influence_index).bone_index;
                        }

                        else
                        {
                            size_t minimum_value_index = 0;
                            float minimum_value = FLT_MAX;
                            for (size_t i = 0; i < MAX_BONE_INFLUENCES; ++i)
                            {
                                if (minimum_value > vertex.bone_weights[i])
                                {
                                    minimum_value = vertex.bone_weights[i];
                                    minimum_value_index = i;
                                }
                            }
                            vertex.bone_weights[minimum_value_index] += influences_per_control_point.at(influence_index).bone_weight;
                            vertex.bone_indices[minimum_value_index] = influences_per_control_point.at(influence_index).bone_index;
                        }
                    }


                    //法線情報の取得
                    if (fbx_mesh->GetElementNormalCount() > 0)
                    {
                        FbxVector4 normal;
                        fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon, normal);
                        vertex.normal.x = static_cast<float>(normal[0]);
                        vertex.normal.y = static_cast<float>(normal[1]);
                        vertex.normal.z = static_cast<float>(normal[2]);
                    }

                    //テクスチャ座標の取得
                    if (fbx_mesh->GetElementUVCount() > 0)
                    {
                        FbxVector2 uv;
                        bool unmapped_uv;
                        fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon,
                            uv_names[0], uv, unmapped_uv);
                        vertex.texcoord.x = static_cast<float>(uv[0]);
                        vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
                    }

                    //法線マップの取得
                    if (fbx_mesh->GenerateTangentsData(0, false))
                    {
                        const FbxGeometryElementTangent* tangent = fbx_mesh->GetElementTangent(0);
                        vertex.tangent.x = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[0]);
                        vertex.tangent.y = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[1]);
                        vertex.tangent.z = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[2]);
                        vertex.tangent.w = static_cast<float>(tangent->GetDirectArray().GetAt(vertex_index)[3]);
                    }


                    mesh.vertices.at(vertex_index) = std::move(vertex);
                    mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;
                    subset.index_count++;
                }
            }

            //バウンディングボックス取得
            for (const Vertex& v : mesh.vertices)
            {
                mesh.bounding_box[0].x = std::min<float>(mesh.bounding_box[0].x, v.position.x);
                mesh.bounding_box[0].y = std::min<float>(mesh.bounding_box[0].y, v.position.y);
                mesh.bounding_box[0].z = std::min<float>(mesh.bounding_box[0].z, v.position.z);
                mesh.bounding_box[1].x = std::max<float>(mesh.bounding_box[1].x, v.position.x);
                mesh.bounding_box[1].y = std::max<float>(mesh.bounding_box[1].y, v.position.y);
                mesh.bounding_box[1].z = std::max<float>(mesh.bounding_box[1].z, v.position.z);
            }
        }
    }

}


void Skinned_Mesh::fetch_material(const FbxSurfaceMaterial* fbx_material, Material& material, const char* propertyName, const char* factorName, const char* filename)
{
    const FbxProperty property = fbx_material->FindProperty(propertyName);
    const FbxProperty factor = fbx_material->FindProperty(factorName);

    if (property.IsValid() && factor.IsValid())
    {
        FbxDouble4 color = property.Get<FbxDouble4>();
        float f = static_cast<float>(factor.Get<FbxDouble>());

        //色情報取得
        material.color.x = static_cast<float>(color[0]);
        material.color.y = static_cast<float>(color[1]);
        material.color.z = static_cast<float>(color[2]);
        material.color.w = 1.0f;
        material.factor = f;
    }

    if (property.IsValid())
    {
        const int numberOfTextures = property.GetSrcObjectCount<FbxFileTexture>();

        if (numberOfTextures)
        {
            const FbxFileTexture* fileTexture = property.GetSrcObject<FbxFileTexture>();

            if (fileTexture)
            {
                const char* _fileName = fileTexture->GetRelativeFileName();

                char allName[256] = { 0 };
                char fbxFileName[64] = { 0 };

                //パス作成
                _splitpath_s(filename, NULL, NULL, fbxFileName, sizeof(fbxFileName), NULL, NULL, NULL, NULL);
                _makepath_s(allName, 256, NULL, fbxFileName, _fileName, NULL);

                material.texture = allName;
            }
        }
    }
}


void Skinned_Mesh::fetch_pbr(const char* filename, std::string mesh_name, Material& material, const char* pbr_name)
{
    std::filesystem::path file_name = filename;

    file_name.replace_extension("fbm");
    file_name += "/";
    file_name += mesh_name;
    file_name += "_";
    file_name += pbr_name;
    file_name += ".png";

    material.texture = file_name.string();
}


void Skinned_Mesh::fetch_skeleton(FbxMesh* fbx_mesh, Skeleton& skeleton)
{
    //バインドポーズの情報を取得
    const int deformer_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
    for (int deformer_index = 0; deformer_index < deformer_count; ++deformer_index)
    {
        FbxSkin* skin = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(deformer_index, FbxDeformer::eSkin));
        const int cluster_count = skin->GetClusterCount();
        skeleton.bones.resize(cluster_count);

        for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
        {
            FbxCluster* cluster = skin->GetCluster(cluster_index);

            //ボーンを取得
            Bone& bone{ skeleton.bones.at(cluster_index) };
            bone.name = cluster->GetLink()->GetName();
            bone.unique_id = cluster->GetLink()->GetUniqueID();
            bone.parent_index = skeleton.indexof_born(cluster->GetLink()->GetParent()->GetUniqueID());
            bone.node_index = indexof_node(bone.unique_id);

            //姿勢行列を取得
            FbxAMatrix reference_global_init_position;
            cluster->GetTransformMatrix(reference_global_init_position);

            FbxAMatrix cluster_global_init_position;
            cluster->GetTransformLinkMatrix(cluster_global_init_position);

            //ローカル空間へ変換
            bone.offset_transform = to_xmfloat4x4(cluster_global_init_position.Inverse() * reference_global_init_position);
        }
    }
}


void Skinned_Mesh::fetch_bone_influences(const FbxMesh* fbx_mesh, std::vector<bone_influences_per_control_point>& bone_influences)
{
    //ボーン影響度の取得
    const int control_points_count{ fbx_mesh->GetControlPointsCount() };
    bone_influences.resize(control_points_count);

    const int skin_count{ fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) };
    for (int skin_index = 0; skin_index < skin_count; ++skin_index)
    {
        const FbxSkin* fbx_skin{ static_cast<FbxSkin*>(fbx_mesh->GetDeformer(skin_index, FbxDeformer::eSkin)) };

        const int cluster_count{ fbx_skin->GetClusterCount() };
        for (int cluster_index = 0; cluster_index < cluster_count; ++cluster_index)
        {
            const FbxCluster* fbx_cluster{ fbx_skin->GetCluster(cluster_index) };

            const int control_point_indices_count{ fbx_cluster->GetControlPointIndicesCount() };
            for (int control_point_indices_index = 0; control_point_indices_index < control_point_indices_count; ++control_point_indices_index)
            {
                int control_point_index{ fbx_cluster->GetControlPointIndices()[control_point_indices_index] };
                double control_point_weight{ fbx_cluster->GetControlPointWeights()[control_point_indices_index] };
                Bone_Influence& bone_influence{ bone_influences.at(control_point_index).emplace_back() };
                bone_influence.bone_index = static_cast<uint32_t>(cluster_index);
                bone_influence.bone_weight = static_cast<float>(control_point_weight);
            }
        }
    }
}


void Skinned_Mesh::fetch_animations(FbxScene* fbx_scene, float sampling_rate /*If this value is 0, the animation data will be sampled at the default frame rate.*/)
{
    //アニメーション情報を取得
    FbxArray<FbxString*> animation_stack_names;
    fbx_scene->FillAnimStackNameArray(animation_stack_names);

    const int animation_stack_count{ animation_stack_names.GetCount() };
    for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index)
    {
        //アニメーションクリップ
        Animation& animation_clip{ animations.emplace_back() };
        animation_clip.name = animation_stack_names[animation_stack_index]->Buffer();

        FbxAnimStack* animation_stack{ fbx_scene->FindMember<FbxAnimStack>(animation_clip.name.c_str()) };
        fbx_scene->SetCurrentAnimationStack(animation_stack);

        const FbxTime::EMode time_mode{ fbx_scene->GetGlobalSettings().GetTimeMode() };
        FbxTime one_second;
        one_second.SetTime(0, 0, 1, 0, 0, time_mode);
        animation_clip.sampling_rate = sampling_rate > 0 ? sampling_rate : static_cast<float>(one_second.GetFrameRate(time_mode));
        

        //再生時間の取得
        const FbxTime sampling_interval{ static_cast<FbxLongLong>(one_second.Get() / animation_clip.sampling_rate) };
        const FbxTakeInfo* take_info{ fbx_scene->GetTakeInfo(animation_clip.name.c_str()) };
        const FbxTime start_time{ take_info->mLocalTimeSpan.GetStart() };
        const FbxTime stop_time{ take_info->mLocalTimeSpan.GetStop() };


        for (FbxTime time = start_time; time < stop_time; time += sampling_interval)
        {
            Keyframe& keyframe{ animation_clip.keyframes.emplace_back() };

            const size_t node_count{ nodes.size() };
            keyframe.node_keys.resize(node_count);

            for (size_t node_index = 0; node_index < node_count; ++node_index)
            {
                FbxNode* fbx_node{ fbx_scene->FindNodeByName(nodes.at(node_index).name.c_str()) };
                
                if (fbx_node)
                {
                    NodeKeyData& node{ keyframe.node_keys.at(node_index) };
                    // 'global_transform' is a transformation matrix of a node with respect to
                    // the scene's global coordinate system.
                    node.global_transform = to_xmfloat4x4(fbx_node->EvaluateGlobalTransform(time));

                    // 'local_transform' is a transformation matrix of a node with respect to
                    // its parent's local coordinate system.
                    const FbxAMatrix& local_transform{ fbx_node->EvaluateLocalTransform(time) };
                    node.scaling = to_xmfloat3(local_transform.GetS());
                    node.rotation = to_xmfloat4(local_transform.GetQ());
                    node.translation = to_xmfloat3(local_transform.GetT());
                }
            }
        }
    }

    for (int animation_stack_index = 0; animation_stack_index < animation_stack_count; ++animation_stack_index)
    {
        delete animation_stack_names[animation_stack_index];
    }
}


void Skinned_Mesh::update_animation(Keyframe& keyframe)
{
    size_t node_count{ nodes.size() };
    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        NodeKeyData& node{ keyframe.node_keys.at(node_index) };
        XMMATRIX S{ XMMatrixScaling(node.scaling.x, node.scaling.y, node.scaling.z) };
        XMMATRIX R{ XMMatrixRotationQuaternion(XMLoadFloat4(&node.rotation)) };
        XMMATRIX T{ XMMatrixTranslation(node.translation.x, node.translation.y, node.translation.z) };

        int64_t parent_index{ nodes.at(node_index).parent_index };
        XMMATRIX P{ parent_index < 0 ? XMMatrixIdentity() :
            XMLoadFloat4x4(&keyframe.node_keys.at(parent_index).global_transform) };

        XMStoreFloat4x4(&node.global_transform, S * R * T * P);
    }
}


bool Skinned_Mesh::append_animations(const char* animation_filename, float sampling_rate)
{
    FbxManager* fbx_manager{ FbxManager::Create() };
    FbxScene* fbx_scene{ FbxScene::Create(fbx_manager, "") };

    FbxImporter* fbx_importer{ FbxImporter::Create(fbx_manager, "") };
    bool import_status{ false };

    import_status = fbx_importer->Initialize(animation_filename);
    _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

    import_status = fbx_importer->Import(fbx_scene);
    _ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

    fetch_animations(fbx_scene, sampling_rate);

    fbx_manager->Destroy();

    return true;
}


void Skinned_Mesh::blend_animations(const Keyframe* keyframes[2], float factor, Keyframe& keyframe)
{
    size_t node_count{ keyframes[0]->node_keys.size() };
    keyframe.node_keys.resize(node_count);

    for (size_t node_index = 0; node_index < node_count; ++node_index)
    {
        XMVECTOR S[2]{
            XMLoadFloat3(&keyframes[0]->node_keys.at(node_index).scaling),
            XMLoadFloat3(&keyframes[1]->node_keys.at(node_index).scaling) };
        XMStoreFloat3(&keyframe.node_keys.at(node_index).scaling, XMVectorLerp(S[0], S[1], factor));

        XMVECTOR R[2]{
            XMLoadFloat4(&keyframes[0]->node_keys.at(node_index).rotation),
            XMLoadFloat4(&keyframes[1]->node_keys.at(node_index).rotation) };
        XMStoreFloat4(&keyframe.node_keys.at(node_index).rotation, XMQuaternionSlerp(R[0], R[1], factor));

        XMVECTOR T[2]{ XMLoadFloat3(&keyframes[0]->node_keys.at(node_index).translation),
            XMLoadFloat3(&keyframes[1]->node_keys.at(node_index).translation) };
        XMStoreFloat3(&keyframe.node_keys.at(node_index).translation, XMVectorLerp(T[0], T[1], factor));
    }
}


void Skinned_Mesh::create_com_objects(ID3D11Device* device, const char* fbx_filename)
{
    for (MeshData& mesh : meshes)
    {
        HRESULT hr{ S_OK };
        D3D11_BUFFER_DESC buffer_desc{};
        D3D11_SUBRESOURCE_DATA subresource_data{};
        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        buffer_desc.CPUAccessFlags = 0;
        buffer_desc.MiscFlags = 0;
        buffer_desc.StructureByteStride = 0;
        subresource_data.pSysMem = mesh.vertices.data();
        subresource_data.SysMemPitch = 0;
        subresource_data.SysMemSlicePitch = 0;

        //頂点バッファの生成
        hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.vertex_buffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


        buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * mesh.indices.size());
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        subresource_data.pSysMem = mesh.indices.data();

        //インデックスバッファの生成
        hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.index_buffer.ReleaseAndGetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }


    HRESULT hr = S_OK;

    D3D11_BUFFER_DESC buffer_desc{};

    buffer_desc.ByteWidth = sizeof(Constants);
    buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    //定数バッファの生成
    hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.ReleaseAndGetAddressOf());
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    //テクスチャの作成
    for (auto& mesh : meshes)
    {
        for (auto& subset : mesh.subsets)
        {
            //ambient
            if (subset.ambient.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.ambient.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.ambient.texture.c_str(), subset.ambient.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }

            //diffuse
            if (subset.diffuse.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.diffuse.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.diffuse.texture.c_str(), subset.diffuse.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }

            //emissive
            if (subset.emissive.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.emissive.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.emissive.texture.c_str(), subset.emissive.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }

            //specular
            if (subset.specular.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.specular.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.specular.texture.c_str(), subset.specular.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }

            //bump
            if (subset.bump.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.bump.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.bump.texture.c_str(), subset.bump.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }

            //normal_map
            if (subset.normal_map.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.normal_map.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.normal_map.texture.c_str(), subset.normal_map.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }


            //albedo
            if (subset.albedo.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.albedo.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.albedo.texture.c_str(), subset.albedo.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }

            //metalness
            if (subset.metalness.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.metalness.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.metalness.texture.c_str(), subset.metalness.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }

            //roughness
            if (subset.roughness.texture[0] == NULL)
            {
                make_dummy_texture(device, subset.roughness.shader_resource_view.GetAddressOf(), 0xFFFFFFFF, 16);   //テクスチャがない場合、ダミーテクスチャを作成
            }
            else
            {
                D3D11_TEXTURE2D_DESC texture2d_desc;
                load_texture_from_file(device, subset.roughness.texture.c_str(), subset.roughness.shader_resource_view.GetAddressOf(), &texture2d_desc);
            }
        }

    }
}


HRESULT Skinned_Mesh::make_dummy_texture(ID3D11Device* device, ID3D11ShaderResourceView** shader_resource_view, DWORD value/*0xAABBGGRR*/, UINT dimension)
{
    HRESULT hr{ S_OK };

    D3D11_TEXTURE2D_DESC texture2d_desc{};
    texture2d_desc.Width = dimension;
    texture2d_desc.Height = dimension;
    texture2d_desc.MipLevels = 1;
    texture2d_desc.ArraySize = 1;
    texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texture2d_desc.SampleDesc.Count = 1;
    texture2d_desc.SampleDesc.Quality = 0;
    texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;


    size_t texels = dimension * dimension;
    unique_ptr<DWORD[]> sysmem{ make_unique<DWORD[]>(texels) };
    for (size_t i = 0; i < texels; ++i) sysmem[i] = value;


    D3D11_SUBRESOURCE_DATA subresource_data{};
    subresource_data.pSysMem = sysmem.get();
    subresource_data.SysMemPitch = sizeof(DWORD) * dimension;


    ComPtr<ID3D11Texture2D> texture2d;

    //テクスチャの生成
    hr = device->CreateTexture2D(&texture2d_desc, &subresource_data, &texture2d);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc{};
    shader_resource_view_desc.Format = texture2d_desc.Format;
    shader_resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    shader_resource_view_desc.Texture2D.MipLevels = 1;

    //シェーダーリソースビューの生成
    hr = device->CreateShaderResourceView(texture2d.Get(), &shader_resource_view_desc, shader_resource_view);
    _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


    return hr;
}


void Skinned_Mesh::render(ID3D11DeviceContext* immediate_context, const XMFLOAT4X4& world, const XMFLOAT4& material_color, const Keyframe* keyframe)
{
    for (MeshData& mesh : meshes)
    {
        uint32_t stride{ sizeof(Vertex) };
        uint32_t offset{ 0 };

        immediate_context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
        immediate_context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        Constants data;
        if (keyframe && keyframe->node_keys.size() > 0)
        {
            const NodeKeyData& mesh_node{ keyframe->node_keys.at(mesh.node_index) };
            XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh_node.global_transform) * XMLoadFloat4x4(&world));

            const size_t bone_count{ mesh.skeletons.bones.size() };
            _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

            for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
            {
                const Bone& bone{ mesh.skeletons.bones.at(bone_index) };
                const NodeKeyData& bone_node{ keyframe->node_keys.at(bone.node_index) };

                XMStoreFloat4x4(&data.bone_transforms[bone_index],
                    XMLoadFloat4x4(&bone.offset_transform) *
                    XMLoadFloat4x4(&bone_node.global_transform) *
                    XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.default_global_transform))
                );
            }
        }

        else
        {
            XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh.default_global_transform) * XMLoadFloat4x4(&world));

            for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
            {
                data.bone_transforms[bone_index] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            }
        }

        for (const Subset& subset : mesh.subsets)
        {
            XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&subset.diffuse.color));

            immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
            immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

            immediate_context->PSSetShaderResources(0, 1, subset.diffuse.shader_resource_view.GetAddressOf());
            immediate_context->PSSetShaderResources(1, 1, subset.normal_map.shader_resource_view.GetAddressOf());

            immediate_context->DrawIndexed(subset.index_count, subset.index_start, 0);
        }
    }
}


void Skinned_Mesh::render_mesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, const Keyframe* keyframe, int mesh_num)
{
    MeshData& mesh = meshes[mesh_num];


    uint32_t stride{ sizeof(Vertex) };
    uint32_t offset{ 0 };

    immediate_context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
    immediate_context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

    Constants data;
    if (keyframe && keyframe->node_keys.size() > 0)
    {
        const NodeKeyData& mesh_node{ keyframe->node_keys.at(mesh.node_index) };
        XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh_node.global_transform) * XMLoadFloat4x4(&world));

        const size_t bone_count{ mesh.skeletons.bones.size() };
        _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

        for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
        {
            const Bone& bone{ mesh.skeletons.bones.at(bone_index) };
            const NodeKeyData& bone_node{ keyframe->node_keys.at(bone.node_index) };

            XMStoreFloat4x4(&data.bone_transforms[bone_index],
                XMLoadFloat4x4(&bone.offset_transform) *
                XMLoadFloat4x4(&bone_node.global_transform) *
                XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.default_global_transform))
            );
        }
    }

    else
    {
        XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh.default_global_transform) * XMLoadFloat4x4(&world));

        for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
        {
            data.bone_transforms[bone_index] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        }
    }

    for (const Subset& subset : mesh.subsets)
    {
        XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&subset.diffuse.color));

        immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
        immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

        immediate_context->PSSetShaderResources(0, 1, subset.diffuse.shader_resource_view.GetAddressOf());
        immediate_context->PSSetShaderResources(1, 1, subset.normal_map.shader_resource_view.GetAddressOf());

        immediate_context->DrawIndexed(subset.index_count, subset.index_start, 0);
    }
}


void Skinned_Mesh::render_exmesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, const Keyframe* keyframe, int mesh_num)
{
    for (MeshData& mesh : meshes)
    {
        if (mesh.unique_id == meshes.at(mesh_num).unique_id) continue;

        uint32_t stride{ sizeof(Vertex) };
        uint32_t offset{ 0 };

        immediate_context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
        immediate_context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

        Constants data;
        if (keyframe && keyframe->node_keys.size() > 0)
        {
            const NodeKeyData& mesh_node{ keyframe->node_keys.at(mesh.node_index) };
            XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh_node.global_transform) * XMLoadFloat4x4(&world));

            const size_t bone_count{ mesh.skeletons.bones.size() };
            _ASSERT_EXPR(bone_count < MAX_BONES, L"The value of the 'bone_count' has exceeded MAX_BONES.");

            for (size_t bone_index = 0; bone_index < bone_count; ++bone_index)
            {
                const Bone& bone{ mesh.skeletons.bones.at(bone_index) };
                const NodeKeyData& bone_node{ keyframe->node_keys.at(bone.node_index) };

                XMStoreFloat4x4(&data.bone_transforms[bone_index],
                    XMLoadFloat4x4(&bone.offset_transform) *
                    XMLoadFloat4x4(&bone_node.global_transform) *
                    XMMatrixInverse(nullptr, XMLoadFloat4x4(&mesh.default_global_transform))
                );
            }
        }

        else
        {
            XMStoreFloat4x4(&data.world, XMLoadFloat4x4(&mesh.default_global_transform) * XMLoadFloat4x4(&world));

            for (size_t bone_index = 0; bone_index < MAX_BONES; ++bone_index)
            {
                data.bone_transforms[bone_index] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
            }
        }

        for (const Subset& subset : mesh.subsets)
        {
            XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&subset.diffuse.color));

            immediate_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
            immediate_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

            immediate_context->PSSetShaderResources(0, 1, subset.diffuse.shader_resource_view.GetAddressOf());
            immediate_context->PSSetShaderResources(1, 1, subset.normal_map.shader_resource_view.GetAddressOf());

            immediate_context->DrawIndexed(subset.index_count, subset.index_start, 0);
        }
    }
}

