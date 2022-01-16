#include "shadow_map.hlsli"



VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	float4 blended_position = { 0, 0, 0, 1 };

	for (int bone_index = 0; bone_index < 4; ++bone_index)
	{
		blended_position += vin.bone_weights[bone_index] * mul(vin.position, bone_transforms[vin.bone_indices[bone_index]]);
	}

	vin.position = float4(blended_position.xyz, 1.0f);

	vout.position = mul(vin.position, mul(world, view_projection));
	vout.texcoord = vin.texcoord;

	float4 depth = { vout.position.z / vout.position.w, 0.0f, 0.0f, 1.0f };
	vout.color = depth;

	return vout;
}


//VS_OUT main(VS_IN vin)
//{
//	VS_OUT vout;
//
//
//	vin.normal.w = 0;
//	float sigma = vin.tangent.w;
//	vin.tangent.w = 0;
//
//	float4 blended_position = { 0, 0, 0, 1 };
//	float4 blended_normal = { 0, 0, 0, 0 };
//	float4 blended_tangent = { 0, 0, 0, 0 };
//
//	for (int bone_index = 0; bone_index < 4; ++bone_index)
//	{
//		blended_position += vin.bone_weights[bone_index] * mul(vin.position, bone_transforms[vin.bone_indices[bone_index]]);
//		blended_normal += vin.bone_weights[bone_index] * mul(vin.normal, bone_transforms[vin.bone_indices[bone_index]]);
//		blended_tangent += vin.bone_weights[bone_index] * mul(vin.tangent, bone_transforms[vin.bone_indices[bone_index]]);
//	}
//
//	vin.position = float4(blended_position.xyz, 1.0f);
//	vin.normal = float4(blended_normal.xyz, 0.0f);
//	vin.tangent = float4(blended_tangent.xyz, 0.0f);
//
//
//	vout.position = mul(vin.position, mul(world, view_projection));
//
//	vout.world_position = mul(vin.position, world);
//	vout.world_normal = normalize(mul(vin.normal, world));
//	vout.world_tangent = normalize(mul(vin.tangent, world));
//	vout.world_tangent.w = sigma;
//
//	vout.world_binormal.xyz = normalize(cross(vout.world_normal.xyz, vout.world_tangent.xyz));
//	vout.world_binormal.w = 0.0f;
//
//	vout.texcoord = vin.texcoord;
//
//	vout.color = material_color;
//
//
//	return vout;
//}
