
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
	//�A�j���[�V�����̎擾
	Model_Resource::Animation& animation{ resource->animations.at(anime_num) };

	//���݂̃A�j���[�V�����Ɠ��ꂩ
    if (now_anime.name != animation.name)
    {
		//���݂̃A�j���[�V�������Ȃ��ꍇ�̓u�����h�⊮���Ȃ�
        if (now_anime.name != "")
        {
			blend_mode = true;
        }

		//�Ⴄ�Ȃ�u�����h�⊮����
		anime_timer = 0.0f;
		now_anime = animation;
		blend_timer = 0.0f;
    }

	//�Đ�����A�j���[�V�����̃t���[�����擾
	int frame_index = static_cast<int>(anime_timer * animation.sampling_rate);
	//�Đ��I����
	if (frame_index > animation.keyframes.size() - 1)
	{
		if (!loop) return;

		frame_index = 0;
		anime_timer = 0.0f;
		blend_mode = true;
		blend_timer = 0.0f;
		anime_play_flag = false;
	}
	//�Đ���
	else
	{
		anime_timer += elapsed_time;
		anime_play_flag = true;
	}

	//�u�����h�⊮
    if (blend_mode)
    {
		//�^�C�}�[
		blend_timer += elapsed_time;

		//�u�����h���Ԃ��A�j���[�V�����̎��Ԃ𒴂��Ă��鎞
		if (static_cast<int>(animation.keyframes.size()) - 1 < blend_second * static_cast<int>(animation.sampling_rate))
		{
			blend_second = (animation.keyframes.size() - 1) / animation.sampling_rate;
		}

		//�⊮��
		float leap = blend_timer / blend_second;

		//�⊮����1.0f�𒴂���ƃu�����h�I��
		if (leap > 1.0f)
		{
			leap = 1.0f;
			blend_timer = blend_second;
			blend_mode = false;
		}

		//�A�j���[�V�����u�����h
		const Model_Resource::Keyframe* keyframes[2]{
			&now_key,
			&animation.keyframes.at(static_cast<int>(blend_timer * 60.0f))
		};

		resource->blend_animations(keyframes, leap, now_key);
		resource->update_animation(now_key);
    }

	//�u�����h�⊮���Ȃ���
	else
	{
		now_key = animation.keyframes.at(frame_index);
	}
}


void Model::render(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
	//�S�Ẵ��b�V����`��
	resource->render(immediate_context, world, material_color, &now_key);
}


void Model::render_mesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num)
{
	//�w�肵�����b�V���̂ݕ`��
	resource->render_mesh(immediate_context, world, material_color, &now_key, mesh_num);
}


void Model::render_exmesh(ID3D11DeviceContext* immediate_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color, int mesh_num)
{
	//�w�肵�����b�V���ȊO��`��
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
		//���b�V���m�[�h�擾
		Model_Resource::NodeKeyData node;
        if (resource->animations.size() > 0)
        {
            node = now_key.node_keys.at(mesh.node_index);
        }

		else
		{
			node.global_transform = mesh.default_global_transform;
		}

		//���C�����[���h��Ԃ��烍�[�J����Ԃ֕ϊ�
		DirectX::XMMATRIX WorldTransform = DirectX::XMLoadFloat4x4(&node.global_transform) * DirectX::XMLoadFloat4x4(&world);
		DirectX::XMMATRIX InverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransform);

		DirectX::XMVECTOR Start = DirectX::XMVector3TransformCoord(WorldStart, InverseWorldTransform);
		DirectX::XMVECTOR End = DirectX::XMVector3TransformCoord(WorldEnd, InverseWorldTransform);
		DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
		DirectX::XMVECTOR Dir = DirectX::XMVector3Normalize(Vec);
		DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);

		//���C�̒���
		float neart;
		DirectX::XMStoreFloat(&neart, Length);

		//�O�p�`(��)�Ƃ̌�������
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

				//�O�p�`�̒��_�𒊏o
				const Model_Resource::Vertex& a = vertices.at(indices.at(index));
				const Model_Resource::Vertex& b = vertices.at(indices.at(index + 1));
				const Model_Resource::Vertex& c = vertices.at(indices.at(index + 2));

				DirectX::XMVECTOR A = DirectX::XMLoadFloat3(&a.position);
				DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&b.position);
				DirectX::XMVECTOR C = DirectX::XMLoadFloat3(&c.position);

				//�O�p�`�̎O�Ӄx�N�g�����Z�o
				DirectX::XMVECTOR AB = DirectX::XMVectorSubtract(B, A);
				DirectX::XMVECTOR BC = DirectX::XMVectorSubtract(C, B);
				DirectX::XMVECTOR CA = DirectX::XMVectorSubtract(A, C);

				//�O�p�`�̖@���x�N�g�����Z�o
				DirectX::XMVECTOR Normal = DirectX::XMVector3Cross(AB, BC);
				Normal = DirectX::XMVector3Normalize(Normal);

				//�O�p�`�̒��_��������ɂ������ꍇ�͈ʒu���͂��ɂ��炵�čČv�Z
				DirectX::XMFLOAT3 Nf;
				DirectX::XMStoreFloat3(&Nf, Normal);
				if (Nf.x == 0.0f && Nf.y == 0.0f && Nf.z == 0.0f)
				{
					DirectX::XMFLOAT3 Plusf = { 0.001f, 0.0f, 0.0f };
					DirectX::XMVECTOR Plus = DirectX::XMLoadFloat3(&Plusf);
					A = DirectX::XMVectorAdd(A, Plus);
					AB = DirectX::XMVectorSubtract(B, A);

					//�O�p�`�̖@���x�N�g�����Z�o
					Normal = DirectX::XMVector3Cross(AB, BC);
					Normal = DirectX::XMVector3Normalize(Normal);
				}

				//���ς̌��ʂ��v���X�Ȃ�Η�����
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Dir, Normal);
				float dot;
				DirectX::XMStoreFloat(&dot, Dot);
				if (dot > 0) continue;

				//���C�ƕ��ʂ̌�_���Z�o
				DirectX::XMVECTOR V = DirectX::XMVectorSubtract(A, Start);
				DirectX::XMVECTOR T = DirectX::XMVectorDivide(DirectX::XMVector3Dot(V, Normal), Dot);
				float t;
				DirectX::XMStoreFloat(&t, T);

				//��_�܂ł̋��������܂łɌv�Z�����ŋߋ������傫�����̓X�L�b�v
				if (t < 0.0f || t > neart) continue;

				DirectX::XMVECTOR Position = DirectX::XMVectorAdd(Start, DirectX::XMVectorScale(Dir, t));

				// ��_���O�p�`�̓����ɂ��邩���� 
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

				// �ŋߋ������X�V
				neart = t;

				//��_�Ɩ@�����X�V
				HitPosition = Position;
				HitNormal = Normal;
				materialIndex = subset.material_index;
			}
		}

		// ���[�J����Ԃ��烏�[���h��Ԃ֕ϊ�
        if (materialIndex >= 0)
        {
			DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(HitPosition, WorldTransform);
			DirectX::XMVECTOR WorldCrossVec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
			DirectX::XMVECTOR WorldCrossLength = DirectX::XMVector3Length(WorldCrossVec);
			float distance;
			DirectX::XMStoreFloat(&distance, WorldCrossLength);

			// �q�b�g���ۑ�
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

