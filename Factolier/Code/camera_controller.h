#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "input_manager.h"

//カメラコントローラー
class Camera_Controller
{
public:
	Camera_Controller(const DirectX::XMFLOAT3* ptr_target_ = nullptr) : ptr_target(ptr_target_) {}
	~Camera_Controller() {}

	//更新処理
	void update(ID3D11DeviceContext* dc, Input_Manager* input_manager, float elapsed_time);

	//ターゲット位置設定
	void set_target(const DirectX::XMFLOAT3& target)	{ this->target = target; }

private:
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	const DirectX::XMFLOAT3*	ptr_target = nullptr;
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 eye = {};
	float sens = 1.0f;

	bool clear_flag = false;
	float clear_timer = 0.0f;

};