
#include "ocean.hlsli"


#define POINT 0
#define LINEAR 1 
#define ANISOTROPIC 2

SamplerState sampler_states[3] : register(s0);

Texture2D diffuse_texture : register(t0);
Texture2D normal_texture : register(t1);


static const float3 k_anbient = { 0.2f, 0.2f, 0.2f };
static const float blend_alpha = 0.5f;

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 diffuse_color1 = diffuse_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord1);
	float4 diffuse_color2 = diffuse_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord2);

	//カラーテクスチャの合成
	float4 diffuse_color = diffuse_color2 * blend_alpha + diffuse_color1 * (1 - blend_alpha);

	float3 E = normalize(pin.world_position.xyz - camera_position.xyz);
	float3 L = normalize(light_direction.xyz);

	//float3 N = normalize(pin.world_normal.xyz);
	float3x3 mat = { normalize(pin.world_tangent.rgb), normalize(pin.world_binormal.rgb), normalize(pin.world_normal.rgb) };

	//法線マップの加算

#if 0
	float3 N = normal_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord1).rgb;
	N += normal_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord2).rgb;

#else
	float3 N1 = normal_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord1).rgb;
	float3 N2 = normal_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord2).rgb;

	float3 N = N2 * blend_alpha + N1 * (1 - blend_alpha);

#endif

	//	ノーマルテクスチャ法線をワールドへ変換
	N = normalize(mul(N * 2.0f - 1.0f, mat));

	float3 ambient = ambient_color.rgb * k_anbient;

	float3 k_s = { 1.0f, 1.0f, 1.0f };


	float3 directional_diffuse = CalcHalfLambert(N, L, light_color.rgb, pin.color.rgb);

	float3 directional_specular = CalcPhongSpecular(N, L, E, light_color.rgb, k_s);


	float4 color = float4(ambient, diffuse_color.a);
	color.rgb += diffuse_color.rgb * directional_diffuse;
	color.rgb += directional_specular;
	color.a = pin.color.a;

	color = CalcFog(color, fog_color, fog_range.xy, length(pin.world_position.xyz - camera_position.xyz));


	return color;
}
