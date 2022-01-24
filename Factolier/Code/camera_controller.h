#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "input_manager.h"
#include "option_manager.h"

//�J�����R���g���[���[
class Camera_Controller
{
public:
	Camera_Controller(const DirectX::XMFLOAT3* ptr_target_ = nullptr) : ptr_target(ptr_target_) {}
	~Camera_Controller() {}

	//�X�V����
	void update(ID3D11DeviceContext* dc, Input_Manager* input_manager, Option_Manager* option_manager, float elapsed_time);

	//�^�[�Q�b�g�ʒu�ݒ�
	void set_target(const DirectX::XMFLOAT3& target)	{ this->target = target; }

	// �X�e�[�W�N���A���Ă����炱�̊֐����Ă�
	void set_clear() { clear_flag = true; }

	// �S�[�����o���I������t���[�����ǂ���
	bool get_is_performance_end() { return is_performance_end; }

private:
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	const DirectX::XMFLOAT3*	ptr_target = nullptr;
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 eye = {};
	float sens = 1.0f;

	bool clear_flag = false;
	bool is_performance_end = false;	// ���o���I������t���[���Ԃ̂�true
	float clear_timer = 0.0f;

};