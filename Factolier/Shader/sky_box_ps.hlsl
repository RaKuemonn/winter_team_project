
#include "sky_box.hlsli"


#define POINT 0
#define LINEAR 1 
#define ANISOTROPIC 2

SamplerState sampler_states[3] : register(s0);

TextureCube sky_box_texture : register(t0);


float4 main(VS_OUT pin) : SV_TARGET
{
	//return float4(1,0,0,1);
	float3 env_vector = normalize(pin.local_position);
	float4 color = sky_box_texture.SampleLevel(sampler_states[2], env_vector, 0);
	return color;
}