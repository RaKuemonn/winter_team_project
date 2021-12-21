#include "default.hlsli"

float4 main(VS_IN vin)
{
	return mul(vin.position, mul(world, view_projection));
}
