#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>
#include <mutex>
#include "effect_manager.h"


class Effect
{
public:
	Effect(const char* filename, std::mutex& mutex, Effect_Manager* effect_manager);
	~Effect();

	//�Đ�
	Effekseer::Handle play(const DirectX::XMFLOAT3& position, Effect_Manager* effect_manager, float scale = 1.0f);

	//��~
	void stop(Effekseer::Handle handle, Effect_Manager* effect_manager);

	//���W�ݒ�
	void set_position(Effekseer::Handle handle, const DirectX::XMFLOAT3& position, Effect_Manager* effect_manager);

	//�X�P�[���ݒ�
	void set_scale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale, Effect_Manager* effect_manager);

private:
	Effekseer::Effect* effekseer_effect = nullptr;
};
