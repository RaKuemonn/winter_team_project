#pragma once

#include <DirectXMath.h>

//�J�����R���g���[���[
class Camera_Controller
{
public:
	Camera_Controller() {}
	~Camera_Controller() {}

	//�X�V����
	void update(float elapsedTime);

	//�^�[�Q�b�g�ʒu�ݒ�
	void set_target(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	float				rollSpeed = DirectX::XMConvertToRadians(50);
	float				range = 10.0f;
	float				maxAngle = DirectX::XMConvertToRadians(45);
	float				minAngle = DirectX::XMConvertToRadians(-45);

};