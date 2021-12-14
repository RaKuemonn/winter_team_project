
struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 world_position : POSITION;

	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 normal : NORMAL;

	float2 texcoord : TEXCOORD;

};

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world;
	float4 ka;
};

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
	row_major float4x4 view_projection;
	float4 camera_position;
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b2)
{
	float4 ambient_color;
	float4 light_direction;
	float4 light_color;
};


cbuffer FOG_CONSTANT_BUFFER : register(b5)
{
	float4 fog_color;
	float4 fog_range;
};


#include "shading_function.hlsli"

