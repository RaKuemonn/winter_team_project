#pragma once

#include <DirectXMath.h>
#include "input_manager.h"

//�J�����R���g���[���[
class Camera_Controller
{
public:
	Camera_Controller(const DirectX::XMFLOAT3* ptr_target_ = nullptr) : ptr_target(ptr_target_) {}
	~Camera_Controller() {}

	//�X�V����
	void update(float elapsedTime, Input_Manager* input_manager);

	//�^�[�Q�b�g�ʒu�ݒ�
	void set_target(const DirectX::XMFLOAT3& target)	{ this->target = target; }

private:
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	const DirectX::XMFLOAT3*	ptr_target = nullptr;
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	float				range = 5.0f;

};