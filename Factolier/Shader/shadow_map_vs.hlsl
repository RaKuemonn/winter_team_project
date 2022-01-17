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
