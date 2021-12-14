
#include "default.hlsli"


#define POINT 0
#define LINEAR 1 
#define ANISOTROPIC 2

SamplerState sampler_states[3] : register(s0);

Texture2D albedo_texture : register(t0);
Texture2D normal_texture : register(t1);
Texture2D metalness_texture : register(t2);
Texture2D roughness_texture : register(t3);
TextureCube specular_texture : register(t4);
TextureCube irradiance_texture : register(t5);
Texture2D specular_brdf_lut : register(t6);

static const float pi = 3.141592;
static const float epsilon = 0.00001;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 f_dielectric = 0.04;


// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndf_ggx(float cos_lh, float roughness)
{
	float alpha = roughness * roughness;
	float alpha_sq = alpha * alpha;

	float denom = (cos_lh * cos_lh) * (alpha_sq - 1.0) + 1.0;
	return alpha_sq / (pi * denom * denom);
}

// Single term for separable Schlick-GGX below.
float ga_schlick_g1(float cos_theta, float k)
{
	return cos_theta / (cos_theta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float ga_schlick_ggx(float cos_li, float cos_lo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return ga_schlick_g1(cos_li, k) * ga_schlick_g1(cos_lo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnel_schlick(float3 f0, float cos_theta)
{
	return f0 + (1.0 - f0) * pow(1.0 - cos_theta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint query_specular_texture_levels()
{
	uint width, height, levels;
	specular_texture.GetDimensions(0, width, height, levels);
	return levels;
}



float4 main(VS_OUT pin) : SV_TARGET
{
	// Sample input textures to get shading model params.
	float3 albedo = albedo_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).rgb;
	float metalness = metalness_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).r;
	float roughness = roughness_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).r;


	// Outgoing light direction (vector from world-space fragment position to the "eye").
	float3 lo = normalize(camera_position.xyz - pin.world_position.xyz);

	// Pass tangent space basis vectors (for normal mapping).
	float3x3 tbn = float3x3(pin.world_tangent.xyz, pin.world_binormal.xyz, pin.world_normal.xyz);

	// Get current fragment's normal and transform to world space.
    float3 n = normalize(2.0 * normal_texture.Sample(sampler_states[ANISOTROPIC], pin.texcoord).rgb - 1.0);
	n = normalize(mul(n, tbn));

	// Angle between surface normal and outgoing light direction.
	float cos_lo = max(0.0, dot(n, lo));

	// Specular reflection vector.
	float3 lr = 2.0 * cos_lo * n - lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
	float3 f0 = lerp(f_dielectric, albedo, metalness);



	// Direct lighting calculation for analytical lights.
	float3 direct_lighting = 0.0;
	{
		float3 li = -light_direction.xyz;
		float3 l_radiance = light_radiance.xyz;

		// Half-vector between Li and Lo.
		float3 lh = normalize(li + lo);

		// Calculate angles between surface normal and various light vectors.
		float cos_li = max(0.0, dot(n, li));
		float cos_lh = max(0.0, dot(n, lh));

		// Calculate Fresnel term for direct lighting. 
		float3 f = fresnel_schlick(f0, max(0.0, dot(lh, lo)));
		// Calculate normal distribution for specular BRDF.
		float d = ndf_ggx(cos_lh, roughness);
		// Calculate geometric attenuation for specular BRDF.
		float g = ga_schlick_ggx(cos_li, cos_lo, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		float3 kd = lerp(float3(1, 1, 1) - f, float3(0, 0, 0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		float3 diffuse_brdf = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
		float3 specular_brdf = (f * d * g) / max(epsilon, 4.0 * cos_li * cos_lo);

		// Total contribution for this light.
		direct_lighting += (diffuse_brdf + specular_brdf) * l_radiance * cos_li;
	}


	// Ambient lighting (IBL).
	float3 ambient_lighting;
	{
		// Sample diffuse irradiance at normal direction.
		float3 irradiance = irradiance_texture.Sample(sampler_states[ANISOTROPIC], n).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
		float3 f = fresnel_schlick(f0, cos_lo);

		// Get diffuse contribution factor (as with direct lighting).
		float3 kd = lerp(1.0 - f, 0.0, metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
		float3 diffuse_ibl = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level.
		uint specular_texture_levels = query_specular_texture_levels();
		float3 specular_irradiance = specular_texture.SampleLevel(sampler_states[ANISOTROPIC], lr, roughness * specular_texture_levels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
		float2 specular_brdf = specular_brdf_lut.Sample(sampler_states[LINEAR], float2(cos_lo, roughness)).rg;

		// Total specular IBL contribution.
		float3 specular_ibl = (f0 * specular_brdf.x + specular_brdf.y) * specular_irradiance;

		// Total ambient lighting contribution.
		ambient_lighting = diffuse_ibl + specular_ibl;
	}



	// Final fragment color.
	return float4(direct_lighting + ambient_lighting, 1.0);
}