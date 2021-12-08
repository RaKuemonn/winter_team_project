
#include "sky_box.hlsli"


VS_OUT main( float4 pos : POSITION )
{
	VS_OUT vout;

	vout.local_position = pos.xyz;
	pos.w = 1.0f;
	vout.world_position = mul(view_rotate, pos);
	vout.world_position = mul(pos,view_rotate);

	return vout;
}