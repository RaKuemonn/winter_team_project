
#include "model.h"
#include "model_manager.h"
#include "collision.h"



Model::Model(std::shared_ptr<Model_Resource> resource_)
{
	resource = resource_;
	
	if (resource->animations.size() > 0)
	{
		now_anime = resource->animations.at(0);
		now_key = now_anime.keyframes.at(0);
	}

	anime_play_flag = false;
}


void Model::play_animation(float elapsed_time, int anime_num, bool loop, float blend_second)
{
	//アニメーションの取得
	Model_Resource::Animation& animation{ resource->animations.at(anime_num) };

	//現在のアニメーションと同一か
    if (now_anime.name != animation.name)
    {
		//現在のアニメーションがない場合はブレンド補完しない
        if (now_anime.name != "")
        {
			blend_mode = true;
        }

		//違うならブレンド補完する
		anime_timer = 0.0f;
		now_anime = animation;
		blend_timer = 0.0f;
    }

	//再生するアニメーションのフレームを取得
	int frame_index = static_cast<int>(anime_timer * animation.sampling_rate);
	//再生終了時
	if (frame_index > animation.keyframes.size() - 1)
	{
		if (!loop) return;

		frame_index = 0;
		anime_timer = 0.0f;
		blend_mode = true;
		blend_timer = 0.0f;
		anime_play_flag = false;
	}
	//再生中
	else
	{
		anime_timer += elapsed_time;
		anime_play_flag = true;
	}

	//ブレンド補完
    if (blend_mode)
    {
		//タイマー
		blend_timer += elapsed_time;

		//ブレンド時間がアニメーションの時間を超えている時
		if (static_cast<int>(animation.keyframes.size()) - 1 < blend_second * static_cast<int>(animation.sampling_rate))
		{
			blend_second = (animation.keyframes.size() - 1) / animation.sampling_rate;
		}

		//補完率
		float leap = blend_timer / blend_second;

		//補完率が1.0fを超えるとブレンド終了
		if (leap > 1.0f)
		{
			leap = 1.0f;
			blend_timer = blend_second;
			blend_mode = false;
		}

		//アニメーションブレンド
		const Model_Resource::Keyframe* keyframes[2]{
			&now_key,
			&animation.keyframes.at(static_cast<int>(blend_timer * 60.0f))
		};

		resource->blend_animations(keyframes, leap, now_key);
		resource->update_animation(now_key);
    }

	//ブレンド補完しない時
	else
	{
		now_key = animation.keyframes.at(frame_index);
	}
}


void Model::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
	//全てのメッシュを描画
	resource->render(immediate_context, world, material_color, &now_key);
}


void Model::render_mesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num)
{
	//指定したメッシュのみ描画
	resource->render_mesh(immediate_context, world, material_color, &now_key, mesh_num);
}


void Model::render_exmesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num)
{
	//指定したメッシュ以外を描画
	resource->render_exmesh(immediate_context, world, material_color, &now_key, mesh_num);
}


bool Model::raycast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, const DirectX::XMFLOAT4X4& world, Hit_Result* hit_result)
{
	using namespace DirectX;

	DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
	DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);
	DirectX::XMVECTOR WorldRayVec = DirectX::XMVectorSubtract(WorldEnd, WorldStart);
	DirectX::XMVECTOR WorldRayLenght = DirectX::XMVector3Length(WorldRayVec);


	bool hit = false;

	for (const Model_Resource::MeshData& mesh : resource->meshes)
	{
		//メッシュノード取得
		Model_Resource::NodeKeyData node;
        if (resource->animations.size() > 0)
        {
            node = now_key.node_keys.at(mesh.node_index);
        }

		else
		{
			node.global_transform = mesh.default_global_transform;
		}

		//レイをワールド空間からローカル空間へ変換
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.global_transform) * DirectX::XMLoadFloat4x4(&world);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
		DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

		//レイの長さ
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//三角形(面)との交差判定
		const std::vector<Model_Resource::Vertex>& vertices = mesh.vertices;
		const std::vector<UINT> indices = mesh.indices;


		int materialIndex = -1;
		DirectX::XMVECTOR HitPosition;
		DirectX::XMVECTOR HitNormal;
		for (const Model_Resource::Subset& subset : mesh.subsets)
		{
			for (UINT i = 0; i < subset.index_count; i += 3)
			{
				UINT index = subset.index_start + i;

				//三角形の頂点を抽出
				const Model_Resource::Vertex& a = vertices.at(indices.at(index));
				const Model_Resource::Vertex& b = vertices.at(indices.at(index + 1));
				const Model_Resource::Vertex& c = vertices.at(indices.at(index + 2));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//三角形の三辺ベクトルを算出
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//三角形の法線ベクトルを算出
				DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);
				Normal = DirectX::XMVector3Normalize(Normal);

				//三角形の頂点が直線上にあった場合は位置を僅かにずらして再計算
				DirectX::XMFLOAT3 Nf;
				DirectX::XMStoreFloat3(&Nf, Normal);
				if (Nf.x == 0.0f && Nf.y == 0.0f && Nf.z == 0.0f)
				{
					DirectX::XMFLOAT3 Plusf = { 0.001f, 0.0f, 0.0f };
					DirectX::XMVECTOR Plus = DirectX::XMLoadFloat3(&Plusf);
					A = DirectX::XMVectorAdd(A, Plus);
					AB = DirectX::XMVectorSubtract(B, A);

					//三角形の法線ベクトルを算出
					Normal = DirectX::XMVector3Cross(AB, BC);
					Normal = DirectX::XMVector3Normalize(Normal);
				}

				//内積の結果がプラスならば裏向き
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, Normal);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot > 0) continue;

				//レイと平面の交点を算出
				DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);
				DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(V, Normal), Dot);
				float t;
				DirectX::XMStoreFloat(&t, T);

				//交点までの距離が今までに計算した最近距離より大きい時はスキップ
				if (t < 0.0f || t > neart) continue;

				DirectX::XMVECTOR Position = DirectX::XMVectorAdd(Start, DirectX::XMVectorScale(Dir, t));

				// 交点が三角形の内側にあるか判定 
				// 1
				DirectX::XMVECTOR V1 = DirectX::XMVectorSubtract(Position, A);
				DirectX::XMVECTOR Cross1 = DirectX::XMVector3Cross(AB, V1);
				DirectX::XMVECTOR Dot1 = DirectX::XMVector3Dot(Cross1, Normal);
				float dot1;
				DirectX::XMStoreFloat(&dot1, Dot1);
				if (dot1 < 0.0f) continue;

				// 2
				DirectX::XMVECTOR V2 = DirectX::XMVectorSubtract(Position, B);
				DirectX::XMVECTOR Cross2 = DirectX::XMVector3Cross(BC, V2);
				DirectX::XMVECTOR Dot2 = DirectX::XMVector3Dot(Cross2, Normal);
				float dot2;
				DirectX::XMStoreFloat(&dot2, Dot2);
				if (dot2 < 0.0f) continue;

				// 3
				DirectX::XMVECTOR V3 = DirectX::XMVectorSubtract(Position, C);
				DirectX::XMVECTOR Cross3 = DirectX::XMVector3Cross(CA, V3);
				DirectX::XMVECTOR Dot3 = DirectX::XMVector3Dot(Cross3, Normal);
				float dot3;
				DirectX::XMStoreFloat(&dot3, Dot3);
				if (dot3 < 0.0f) continue;

				// 最近距離を更新
				neart = t;

				//交点と法線を更新
				HitPosition = Position;
				HitNormal = Normal;
				materialIndex = subset.material_index;
			}
		}

		// ローカル空間からワールド空間へ変換
        if (materialIndex >= 0)
        {
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			// ヒット情報保存
			if (hit_result->distance > distance)
			{
				DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(HitNormal, WorldTransform);
			
				hit_result->distance = distance;
				hit_result->material_index = materialIndex;
				DirectX::XMStoreFloat3(&hit_result->position, WorldPosition);
				DirectX::XMStoreFloat3(&hit_result->normal, DirectX::XMVector3Normalize(WorldNormal));
				hit = true;
			}
        }
		
	}

	return hit;
}

