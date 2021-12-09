#pragma once

#include <DirectXMath.h>
#include "d3d11.h"
#include "wrl.h"


class Camera
{
private:
	Camera() {};
	~Camera() {};

public:
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}


	//指定方向を向く
	void set_lookat(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);
	//パースペクティブ設定
	void set_perspective_fov(float fovY, float aspect, float nearZ, float farZ);
	//シーンコンスタント設定
	void set_constants(ID3D11DeviceContext* immediate_context);

	const DirectX::XMFLOAT4X4& get_view() const { return view; }
	const DirectX::XMFLOAT4X4& get_projection() const { return projection; }
	const DirectX::XMFLOAT3& get_eye() const { return eye; }
	const DirectX::XMFLOAT3& get_focus() const { return focus; }
	const DirectX::XMFLOAT3& get_up() const { return up; }
	const DirectX::XMFLOAT3& get_front() const { return front; }
	const DirectX::XMFLOAT3& get_right() const { return right; }

private:
	DirectX::XMFLOAT4X4		view {};
	DirectX::XMFLOAT4X4		projection {};

	DirectX::XMFLOAT3		eye {};
	DirectX::XMFLOAT3		focus {};

	DirectX::XMFLOAT3		up {};
	DirectX::XMFLOAT3		front {};
	DirectX::XMFLOAT3		right {};

};
