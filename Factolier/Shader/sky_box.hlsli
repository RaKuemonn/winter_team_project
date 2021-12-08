
struct VS_OUT
{
    float3 local_position : POSITION;
    float4 world_position : SV_POSITION;
};


cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;	    //ビュー・プロジェクション変換行列
    row_major float4x4 view_rotate;         //ビューの回転行列
    float4 light_direction;		            //ライトの向き
    float4 light_radiance;		            //ライトの向き
    float4 camera_position;		            //カメラ位置
};