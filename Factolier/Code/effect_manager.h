#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>


class Effect_Manager
{
public:
	Effect_Manager(ID3D11Device* device, ID3D11DeviceContext* context) { Initialize(device, context); }
	~Effect_Manager() { finalize(); }

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
	void finalize();
	void update(float elapsedTime);
	void render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	//Effekseerマネージャーの取得
	Effekseer::Manager* get_effekseer_manager() { return effekseer_manager; }

private:
	Effekseer::Manager* effekseer_manager = nullptr;
	EffekseerRenderer::Renderer* effekseer_renderer = nullptr;

};
