
#include "default.hlsli"


#define POINT 0
#define LINEAR 1 
#define ANISOTROPIC 2

SamplerState sampler_states[3] : register(s0);

SamplerState shadow_sampler_state : register(s4);


Texture2D diffuse_texture : register(t0);
Texture2D normal_texture : register(t1);
Texture2D shadow_map : register(t2);


static const float3 k_anbient = { 0.2f, 0.2f, 0.2f };

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 diffuse_color = diffuse_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord);

	// Inverse gamma process
	const float GAMMA = 2.2;
	diffuse_color.rgb = pow(diffuse_color.rgb, GAMMA);
	diffuse_color.rgb = pow(diffuse_color.rgb, 1.0f / GAMMA);


	float3 E = normalize(pin.world_position.xyz - camera_position.xyz);
	float3 L = normalize(light_direction.xyz);

	//float3 N = normalize(pin.world_normal.xyz);
	float3x3 mat = { normalize(pin.world_tangent.rgb), normalize(pin.world_binormal.rgb), normalize(pin.world_normal.rgb) };
	float3 N = normal_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).rgb;
	//	ノーマルテクスチャ法線をワールドへ変換
	N = normalize(mul(N * 2.0f - 1.0f, mat));

	float3 ambient = ambient_color.rgb * k_anbient;

	float3 k_s = { 1.0f, 1.0f, 1.0f };


	float3 directional_diffuse = CalcHalfLambert(N, L, light_color.rgb, pin.color.rgb);
	//float3 directional_diffuse = CalcLambert(N, L, light_color.rgb, pin.color.rgb);

	float3 directional_specular = CalcPhongSpecular(N, L, E, light_color.rgb, k_s);


	float4 color = float4(ambient, diffuse_color.a);
	color.rgb += diffuse_color.rgb * directional_diffuse;
	color.rgb += directional_specular;
	color.a *= pin.color.a;
	
	color = CalcFog(color, fog_color, fog_range.xy, length(pin.world_position.xyz - camera_position.xyz));


	// シャドウマップから深度値取得
	float depth = shadow_map.Sample(shadow_sampler_state, pin.shadow_texcoord.xy).r;
	// 深度値を比較して影かどうかを判定する
	if (pin.shadow_texcoord.z - depth > shadow_bias)
	{
		color.rgb *= shadow_color.rgb;
	}


	return color;
}
