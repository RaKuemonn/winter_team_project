#pragma once

#include <DirectXMath.h>
#include "input_manager.h"

//�J�����R���g���[���[
class Camera_Controller
{
public:
	Camera_Controller() {}
	~Camera_Controller() {}

	//�X�V����
	void update(float elapsedTime, Input_Manager* input_manager);

	//�^�[�Q�b�g�ʒu�ݒ�
	void set_target(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	float				roll_speed = DirectX::XMConvertToRadians(50);
	float				range = 5.0f;
	float				max_angle = DirectX::XMConvertToRadians(80);
	float				min_angle = DirectX::XMConvertToRadians(-80);

};