#pragma once

#include <DirectXMath.h>

//カメラコントローラー
class Camera_Controller
{
public:
	Camera_Controller() {}
	~Camera_Controller() {}

	//更新処理
	void update(float elapsedTime);

	//ターゲット位置設定
	void set_target(const DirectX::XMFLOAT3& target) { this->target = target; }

private:
	DirectX::XMFLOAT3	target = { 0, 0, 0 };
	DirectX::XMFLOAT3	angle = { 0, 0, 0 };
	float				rollSpeed = DirectX::XMConvertToRadians(50);
	float				range = 10.0f;
	float				maxAngle = DirectX::XMConvertToRadians(45);
	float				minAngle = DirectX::XMConvertToRadians(-45);

};