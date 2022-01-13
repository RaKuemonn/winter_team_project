#pragma once

#include <d3d11.h>
#include <wrl.h>
#include "utility.h"


enum class SS
{
	POINT = 0,
	LINEAR,
	ANISOTROPIC,
};

enum class DS
{
	ON_ON = 0,
	ON_OFF,
	OFF_ON,
	OFF_OFF,
};

enum class BS
{
	NONE = 0,
	ALPHA,
	ADD,
	SUBTRACT,
	REPLACE,
	MULTIPLY,
	LIGHTEN,
	DARKEN,
	SCREEN,
	COVERAGE,
};

enum class RS
{
	SOLID_BACK = 0,
	SOLID_NONE,
	WIRE_BACK,
	WIRE_NONE,
};


class State_Manager
{
public:
	State_Manager(ID3D11Device* device, ID3D11DeviceContext* context, HRESULT hr) { generate_state(device, hr); set_context(context); }
	~State_Manager() {};

	void generate_state(ID3D11Device* device, HRESULT hr);
	void set_context(ID3D11DeviceContext* context) { immediate_context = context; }

	void setSS(SS state) { immediate_context->PSSetSamplers(CAST_I(state), 1, sampler_states[CAST_I(state)].GetAddressOf()); }
	void setDS(DS state) { immediate_context->OMSetDepthStencilState(depth_stencil_state[CAST_I(state)].Get(), 1); }
	void setBS(BS state) { immediate_context->OMSetBlendState(blend_states[CAST_I(state)].Get(), nullptr, 0xFFFFFFFF); }
	void setRS(RS state) { immediate_context->RSSetState(rasterizer_state[CAST_I(state)].Get()); }


private:
	ID3D11DeviceContext* immediate_context;


	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state[4];

	Microsoft::WRL::ComPtr<ID3D11BlendState> blend_states[10];

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state[4];
};
