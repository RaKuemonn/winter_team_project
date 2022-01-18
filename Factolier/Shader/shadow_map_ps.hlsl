
#include "shadow_map.hlsli"


#define POINT 0
#define LINEAR 1 
#define ANISOTROPIC 2

SamplerState sampler_states[3] : register(s0);

Texture2D diffuse_texture : register(t0);


float4 main(VS_OUT pin) : SV_TARGET
{
	float4 diffuse_color = diffuse_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);

    pin.color.a = diffuse_color.a;

	return pin.color;
}