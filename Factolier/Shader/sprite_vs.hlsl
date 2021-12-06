
#include "sprite.hlsli"

//受け取った頂点座標と色を構造体として返す
VS_OUT main( float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD )
{
	VS_OUT vout;
	vout.position = position;
	vout.color = color;

	vout.texcoord = texcoord;

	return vout;
}