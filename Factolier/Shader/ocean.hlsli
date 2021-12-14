
struct VS_IN
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 texcoord : TEXCOORD;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
};


struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 world_position : POSITION;
    float4 world_normal : NORMAL;
    float4 world_binormal : BINORMAL;
    float4 world_tangent : TANGENT;
    float2 texcoord1 : TEXCOORD1;
    float2 texcoord2 : TEXCOORD2;
    float4 color : COLOR;
};


static const int MAX_BONES = 256;

cbuffer OBJECT_CONSTANT_BUFFER : register(b0)
{
    row_major float4x4 world;
    float4 material_color;
    row_major float4x4 bone_transforms[MAX_BONES];
};

cbuffer SCENE_CONSTANT_BUFFER : register(b1)
{
    row_major float4x4 view_projection;	    //ビュー・プロジェクション変換行列
    row_major float4x4 view_rotate;         //ビューの回転行列
    float4 camera_position;		            //カメラ位置
    float4 timer;                           //経過時間
};

cbuffer LIGHT_CONSTANT_BUFFER : register(b2)
{
    float4 ambient_color;
    float4 light_direction;		            //ライトの向き
    float4 light_radiance;		            //ライトの向き
    float4 light_color;
};

cbuffer FOG_CONSTANT_BUFFER : register(b3)
{
    float4 fog_color;
    float4 fog_range;
};


#include "shading_function.hlsli"