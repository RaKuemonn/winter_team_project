#pragma once
#include <DirectXMath.h>

struct Hit_Result
{
    DirectX::XMFLOAT3   position        = { 0,0,0 };    // 当てたレイキャストと当たったポリゴンの交点
    DirectX::XMFLOAT3   normal          = { 0,0,0 };    // 当たったポリゴンの法線ベクトル
    float               distance        = 0.0f;                   // レイキャストの始点~終点までの距離
    int                 material_index  = -1;                     // 当たったポリゴンのマテリアル番号
    DirectX::XMFLOAT3   rotation        = { 0,0,0 };    // 回転量
};